#include "handler.hpp"
#include "../common/sfml_overload.hpp"
#include "../server/datamanager/memberentry.hpp"
#include "clientnetwork.hpp"
#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <SFML/Network.hpp>
#include <iostream>
#include <qstringview.h>
#include <unordered_map>

ClientHandler::ClientHandler(ClientNetworkManager &client_manager,
                             ReceiverInWindow &w)
    : manager_(client_manager), handleWindow_(w) {}

void ClientHandler::processEventQueu() {
  while ((manager_).hasEvent()) {
    ServerEvent event = (manager_).popEvent();
    process(event);
  }
}

void ClientHandler::process(ServerEvent &event) {
  switch (event.message_type_) {
  case MsgProtocole::AUTH_RESULT: {
    uint8_t accept;
    uint userId;
    *(event.data_packet_) >> accept >> userId;
    handleWindow_.switchConnectState(accept);
    handleWindow_.setUserId(userId);
    break;
  }

  case MsgProtocole::LOB_PROJECT_LIST_REP: {
    uint32_t size;
    *(event.data_packet_) >> size;
    handleWindow_.clearProjList();

    for (uint i = 0; i < static_cast<uint>(size); ++i) {
      ProjectData projet;
      uint32_t id;
      std::string name;
      uint8_t role;

      *(event.data_packet_) >> id >> name >> role;
      projet.projectId = id;
      projet.projectName = name;
      projet.role = role;

      handleWindow_.addProjectToList(projet);
    }

    break;
  }

  case MsgProtocole::LOB_EXPORT_NATIVE_PROJECT_REP: {
    uint32_t zipSize;
    std::string projName;
    std::string destPathStr;

    *(event.data_packet_) >> zipSize >> projName >> destPathStr;

    // Offset of what preceeds zipBytes
    const char *rawBuf =
        static_cast<const char *>(event.data_packet_->getData());
    size_t readPos = event.data_packet_->getReadPosition();

    // Getting zipBytes compressed by QT
    QByteArray compressedBytes(rawBuf + readPos, zipSize);

    // Uncompressing zipBytes from QT
    QByteArray zipBytes = qUncompress(compressedBytes);

    QString destPath = QString::fromStdString(destPathStr);
    QFile zipFile(destPath);

    // Writing in file with QT
    if (zipFile.open(QIODevice::WriteOnly)) {
      zipFile.write(zipBytes);
      zipFile.close();
    }
    break;
  }

  case MsgProtocole::LOB_GET_PROJECT_DATA_REP: {
    std::uint32_t jsonSize;
    *(event.data_packet_) >> jsonSize;

    // Offset brut : 1 octet type (déjà lu par clientnetwork) + 4 octets
    // taille
    const size_t headerOffset = sizeof(std::uint8_t) + sizeof(std::uint32_t);
    const char *rawBuf = (const char *)(*(event.data_packet_)).getData();
    const size_t totalSize = event.data_packet_->getDataSize();

    auto readU32BE = [](const char *buf, size_t p) -> uint32_t {
      return (static_cast<uint32_t>(static_cast<uint8_t>(buf[p])) << 24) |
             (static_cast<uint32_t>(static_cast<uint8_t>(buf[p + 1])) << 16) |
             (static_cast<uint32_t>(static_cast<uint8_t>(buf[p + 2])) << 8) |
             static_cast<uint32_t>(static_cast<uint8_t>(buf[p + 3]));
    };

    // 1. Décompression et parsing du JSON d'en-tête
    QByteArray jsonBytes =
        qUncompress(QByteArray(rawBuf + headerOffset, jsonSize));
    if (jsonBytes.isEmpty()) {
      std::cerr << "Erreur : décompression du JSON projet échouée."
                << std::endl;
      break;
    }
    QJsonObject entete = QJsonDocument::fromJson(jsonBytes).object();

    // 2. Index { layer_id -> {x, y} } pour retrouver les décalages
    // sauvegardés
    struct LayerMeta {
      int x, y;
      std::string name;
    };
    std::unordered_map<uint, LayerMeta> layerMeta;
    QJsonArray layersJson = entete["layers"].toArray();
    for (const auto &lv : layersJson) {
      QJsonObject lo = lv.toObject();
      uint lid = static_cast<uint>(lo["id"].toInt());
      layerMeta[lid] = {lo["x"].toInt(), lo["y"].toInt(),
                        lo["name"].toString().toStdString()};
    }

    size_t pos = headerOffset + jsonSize;

    Chat chat;
    if (pos + 4 <= totalSize) {
      uint32_t jsonSize2 = readU32BE(rawBuf, pos);
      pos += 4; // On avance de 4 octets après avoir lu la taille
      if (pos + jsonSize2 <= totalSize) {
        QByteArray jsonBytes2 =
            qUncompress(QByteArray(rawBuf + pos, jsonSize2));
        if (!jsonBytes2.isEmpty()) {
          QJsonArray chatArray = QJsonDocument::fromJson(jsonBytes2).array();
          chat = Chat(chatArray);

        } else {
          std::cerr << "Erreur : décompression du JSON chat échouée."
                    << std::endl;
        }
        pos += jsonSize2;
      }
    }

    std::map<uint, sf::Texture> textureMap;
    if (pos + 4 <= totalSize) {
      uint32_t textureCount = readU32BE(rawBuf, pos);
      pos += 4;
      for (uint32_t i = 0; i < textureCount; ++i) {
        // CORRECTION ICI : on vérifie 12 octets (id + width + height)
        if (pos + 12 <= totalSize) { 
          uint32_t textureId = readU32BE(rawBuf, pos);
          pos += 4;
          uint32_t width = readU32BE(rawBuf, pos);
          pos += 4;
          uint32_t height = readU32BE(rawBuf, pos);
          pos += 4;

          // Calcul de la taille du bloc de pixels (RGBA)
          uint32_t pixelSize = width * height * 4;

          if (pos + pixelSize <= totalSize) {
            sf::Image image({width, height}, reinterpret_cast<const uint8_t*>(rawBuf + pos));
            
            sf::Texture texture;
            if (texture.loadFromImage(image)) { 
                textureMap[textureId] = std::move(texture);
            }
            pos += pixelSize;
          }
        }
      }
    }

    // 3. Lecture big-endian des données binaires de chaque layer
    std::vector<LayerLoadData> layers;
    while (pos + 9 <= totalSize) { // min 4 (id) + 1 (type) + 4 (taille)
      LayerLoadData ld;
      ld.id = readU32BE(rawBuf, pos);
      pos += 4;
      ld.type = static_cast<uint8_t>(rawBuf[pos]);
      pos += 1;
      uint32_t dataSize = readU32BE(rawBuf, pos);
      pos += 4;

      if (pos + dataSize > totalSize)
        break;
      ld.data = QByteArray(rawBuf + pos, dataSize);
      pos += dataSize;

      auto it = layerMeta.find(ld.id);
      ld.x = (it != layerMeta.end()) ? it->second.x : 0;
      ld.y = (it != layerMeta.end()) ? it->second.y : 0;
      ld.name = (it != layerMeta.end()) ? it->second.name
                                        : "Layer " + std::to_string(ld.id);

      layers.push_back(std::move(ld));
    }

    // 4. Reconstruction du projet avec tous ses layers (pas de layer par
    // défaut)
    sf::Vector2u vec{static_cast<uint>(entete["width"].toInt()),
                     static_cast<uint>(entete["height"].toInt())};
    handleWindow_.addProjectData(
        entete["scale"].toInt(), vec, entete["name"].toString().toStdString(),
        entete["id"].toInt(), static_cast<uint>(entete["nextLayerId"].toInt()),
        layers, chat, textureMap);
    break;
  }

  case MsgProtocole::LOB_RENAME_PROJECT_REP: {
    uint8_t success;
    uint32_t projectId;
    std::string newName;
    *(event.data_packet_) >> success >> projectId >> newName;
    if (success) {
      handleWindow_.updateProjectNameInList(projectId, newName);
    }
    break;
  }

  case MsgProtocole::LOB_DUPLICATE_PROJECT_REP: {
    uint32_t projectId;
    std::string newName;
    *(event.data_packet_) >> projectId >> newName;
    ProjectData projet;

    projet.projectId = projectId;
    projet.projectName = newName;
    // Owner
    projet.role = 2;

    handleWindow_.addProjectToList(projet);

    break;
  }

  case MsgProtocole::LOB_CREATE_PROJECT_REP: {
    uint32_t newProjectId;
    *(event.data_packet_) >> newProjectId;
    handleWindow_.updateCreatedProjectId(newProjectId);
    break;
  }

  case MsgProtocole::LOB_SHARE_PROJECT_REP: {
    std::string shareToken;
    *(event.data_packet_) >> shareToken;
    handleWindow_.updateShareToken(shareToken);
    break;
  }

  case MsgProtocole::PROJ_GET_MEMBERS_REP: {
    uint32_t size;
    *(event.data_packet_) >> size;
    handleWindow_.clearMemberList();

    std::vector<MemberEntry> memberList;
    for (uint i = 0; i < static_cast<uint>(size); ++i) {
      MemberEntry member;
      std::string pseudo;
      uint32_t role;
      uint32_t userId;

      *(event.data_packet_) >> pseudo >> role >> userId;
      member.pseudo = pseudo;
      member.role = role;
      member.userId = userId;
      memberList.push_back(member);
    }
    handleWindow_.setMemberList(memberList);
    break;
  }

  case MsgProtocole::PROJ_CHANGE_ROLE_REP: {
    bool success;
    *(event.data_packet_) >> success;
    if (success) {
      std::cout << "=== Congrats Roles Changed !! ===" << std::endl;
      uint projectId;
      uint targetId;
      int8_t newrole;
      *(event.data_packet_) >> projectId >> targetId >> newrole;

      handleWindow_.updateMemberList(projectId, targetId, newrole);
    }
    break;
  }
  case MsgProtocole::PROJ_KICK_USER_REP: {
    bool success;
    *(event.data_packet_) >> success;
    std::cout << "kicked if success = " << success << std::endl;
    if (success) {
      uint targetId;
      uint projectId;
      *(event.data_packet_) >> targetId >> projectId;
      handleWindow_.kickUser(targetId);
    }
    break;
  }

  case MsgProtocole::LOB_JOIN_PROJECT_REP: {
    bool success;
    *(event.data_packet_) >> success;
    if (success) {
      manager_.getProjectList();

    } else {
      std::cerr << "Erreur : Token invalide ou projet inexistant." << std::endl;
    }
    // Don't know what to do

    break;
  }
  case MsgProtocole::MAP_CREATE_LAYER_REP: {
    uint project_id;
    uint current_layer_id;
    uint8_t type_int;
    *(event.data_packet_) >> project_id >> type_int;
    LayerType type = static_cast<LayerType>(type_int);
    handleWindow_.createLayer(type);
    break;
  }
  case MsgProtocole::MAP_REMOVE_LAYER_REP: {
    uint project_id;
    uint current_layer_id;
    *(event.data_packet_) >> project_id >> current_layer_id;
    handleWindow_.deleteLayer(current_layer_id);
    break;
  }

  case MsgProtocole::MAP_RENAME_LAYER_REP: {
    uint project_id;
    uint layer_id;
    std::string name;
    *(event.data_packet_) >> project_id >> layer_id >> name;
    handleWindow_.renameLayer(layer_id, name);

    break;
  }

  case MsgProtocole::MAP_ORGANIZE_LAYER_UP_REP: {
    uint project_id;
    uint current_layer_id;
    *(event.data_packet_) >> project_id >> current_layer_id;
    handleWindow_.layerUp(current_layer_id);
    break;
  }

  case MsgProtocole::MAP_ORGANIZE_LAYER_DOWN_REP: {
    uint project_id;
    uint current_layer_id;
    *(event.data_packet_) >> project_id >> current_layer_id;
    handleWindow_.layerDown(current_layer_id);
    break;
  }

  case MsgProtocole::MAP_PUT_PIXELS_CIRCLE_REP: {
    uint project_id;
    uint layer_id;
    int pos_x;
    int pos_y;
    float size;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    *(event.data_packet_) >> project_id >> layer_id >> pos_x >> pos_y >> size >>
        r >> g >> b >> a;
    handleWindow_.drawPixelBrush(layer_id, pos_x, pos_y, r, g, b, a,
                                 Shape::CIRCLE, false, size, 0);
    break;
  }

  case MsgProtocole::MAP_PUT_PIXELS_SQUARE_REP: {
    uint project_id;
    uint layer_id;
    int pos_x;
    int pos_y;
    float size;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    *(event.data_packet_) >> project_id >> layer_id >> pos_x >> pos_y >> size >>
        r >> g >> b >> a;
    handleWindow_.drawPixelBrush(layer_id, pos_x, pos_y, r, g, b, a,
                                 Shape::SQUARE, false, size, 0);
    break;
  }

  case MsgProtocole::MAP_PUT_PIXELS_DIAM_REP: {
    uint project_id;
    uint layer_id;
    int pos_x;
    int pos_y;
    float size_x;
    float size_y;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    *(event.data_packet_) >> project_id >> layer_id >> pos_x >> pos_y >>
        size_x >> size_y >> r >> g >> b >> a;
    handleWindow_.drawPixelBrush(layer_id, pos_x, pos_y, r, g, b, a,
                                 Shape::DIAMOND, false, size_x, size_y);
    break;
  }

  case MsgProtocole::MAP_ERASE_PIXELS_CIRCLE_REP: {
    uint project_id;
    uint layer_id;
    int pos_x;
    int pos_y;
    float size;
    *(event.data_packet_) >> project_id >> layer_id >> pos_x >> pos_y >> size;
    handleWindow_.drawPixelBrush(layer_id, pos_x, pos_y, 0, 0, 0, 0,
                                 Shape::CIRCLE, true, size, 0);
    break;
  }

  case MsgProtocole::MAP_PUT_SPRITE_REP: {
    uint project_id;
    uint layer_id;
    std::string asset_id;
    int pos_x;
    int pos_y;
    float size;
    *(event.data_packet_) >> project_id >> layer_id >> asset_id >> pos_x >>
        pos_y >> size;
    handleWindow_.drawSprite(layer_id, asset_id, pos_x, pos_y, size);
    break;
  }

  case MsgProtocole::MAP_ERASE_PIXELS_SQUARE_REP: {
    uint project_id;
    uint layer_id;
    int pos_x;
    int pos_y;
    float size;
    *(event.data_packet_) >> project_id >> layer_id >> pos_x >> pos_y >> size;
    handleWindow_.drawPixelBrush(layer_id, pos_x, pos_y, 0, 0, 0, 0,
                                 Shape::SQUARE, true, size, 0);
    break;
  }

  case MsgProtocole::MAP_ERASE_PIXELS_DIAM_REP: {
    uint project_id;
    uint layer_id;
    int pos_x;
    int pos_y;
    float size_x;
    float size_y;
    *(event.data_packet_) >> project_id >> layer_id >> pos_x >> pos_y >>
        size_x >> size_y;
    handleWindow_.drawPixelBrush(layer_id, pos_x, pos_y, 0, 0, 0, 0,
                                 Shape::DIAMOND, true, size_x, size_y);
    break;
  }
  case MsgProtocole::MAP_ERASE_SPRITE_REP: {
    uint project_id;
    uint layer_id;
    uint sprite_id;
    *(event.data_packet_) >> project_id >> layer_id >> sprite_id;
    handleWindow_.eraseSprite(layer_id, sprite_id);
    break;
  }

  case MsgProtocole::MAP_MOV_SPRITE_REP: {
    uint project_id;
    uint layer_id;
    uint count;
    sf::Vector2i v;

    *(event.data_packet_) >> project_id >> layer_id >> count >> v.x >> v.y;
    for (uint i = 0; i < count; ++i) {
      uint sprite_id;
      *(event.data_packet_) >> sprite_id;
      handleWindow_.moveSprite(layer_id, sprite_id, v);
    }
    break;
  }

  case MsgProtocole::MAP_RESIZE_SPRITE_REP: {
    uint project_id;
    uint layer_id;
    uint count;
    float scale;

    *(event.data_packet_) >> project_id >> layer_id >> count >> scale;
    for (uint i = 0; i < count; ++i) {
      uint sprite_id;
      sf::Vector2f pos;
      *(event.data_packet_) >> sprite_id >> pos.x >> pos.y;
      handleWindow_.resizeSprite(layer_id, sprite_id, pos, scale);
    }
    break;
  }

  case MsgProtocole::MAP_ROTATE_SPRITE_REP: {
    uint project_id;
    uint layer_id;
    uint count;
    float angle;
    *(event.data_packet_) >> project_id >> layer_id >> count >> angle;

    for (uint i = 0; i < count; ++i) {
      uint sprite_id;
      sf::Vector2f pos;
      *(event.data_packet_) >> sprite_id >> pos.x >> pos.y;
      handleWindow_.rotateSprite(layer_id, sprite_id, angle, pos);
    }
    break;
  }

  case MsgProtocole::MAP_AUTOFILL_REP:{
      uint project_id;
      uint layer_id;
      uint count;
      float rotation;
      float size;
      std::vector<std::string> asset_ids;
      std::vector<sf::Vector2f> positions;
      *(event.data_packet_) >> project_id >> layer_id >> count >> rotation >> size;

      for(int i = 0; i < count; i++){
        std::string id;
        *(event.data_packet_) >> id;
        asset_ids.push_back(id);
      }

      for(int i = 0; i < count; i++){
        sf::Vector2f pos;
        *(event.data_packet_) >> pos.x >> pos.y;
        positions.push_back(pos);
      }

      handleWindow_.autoFill(layer_id,asset_ids,positions,rotation,size);
      break;
    }

  case MsgProtocole::MAP_MOV_LAYER_REP: {
    uint project_id;
    uint layer_id;
    int delta_x;
    int delta_y;
    *(event.data_packet_) >> project_id >> layer_id >> delta_x >> delta_y;
    handleWindow_.shiftLayer(layer_id, delta_x, delta_y);
    break;
  }

  case MsgProtocole::CHAT_MESSAGE_REP: {
    std::string pseudo;
    std::string message;
    int min;
    int hour;
    int day;
    int month;
    int year;
    *(event.data_packet_) >> pseudo >> message >> min >> hour >> day >> month >>
        year;
    handleWindow_.addChatMess(pseudo, message, min, hour, day, month, year);

    break;
  }

  case MsgProtocole::CHAT_SYSTEME_REP: {
    std::string pseudo;
    uint8_t type;
    int min;
    int hour;
    int day;
    int month;
    int year;
    *(event.data_packet_) >> pseudo >> type >> min >> hour >> day >> month >>
        year;
    handleWindow_.addChatSyst(pseudo, type, min, hour, day, month, year);

    break;
  }

  case MsgProtocole::MAP_ADD_SPRITE_REP: {
    sf::Texture sprite;
    uint spriteId;
    *(event.data_packet_) >> spriteId >> sprite;
    handleWindow_.addSprite(spriteId, sprite);

    break;
  }
  }
}
