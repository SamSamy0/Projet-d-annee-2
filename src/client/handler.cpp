#include "handler.hpp"
#include "clientnetwork.hpp"
#include <QByteArray>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <SFML/Network.hpp>
#include <iostream>

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
    *(event.data_packet_) >> accept;
    handleWindow_.switchConnectState(accept);
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

  case MsgProtocole::LOB_GET_PROJECT_DATA_REP: {

    std::uint32_t jsonSize;
    // On suppose que l'octet d'en-tête (MsgProtocole) a déjà été extrait du
    // flux (stream >>) juste avant pour déclencher cet événement. On lit donc
    // la taille.
    *(event.data_packet_) >> jsonSize;

    // 1. Calcul du BON offset (1 octet pour le type de message + 4 octets
    // pour la taille)
    size_t offset = sizeof(std::uint8_t) + sizeof(std::uint32_t);
    const char *ptrDonnees =
        (const char *)(*(event.data_packet_)).getData() + offset;

    // 2. Récupération des données COMPRESSÉES
    QByteArray donneesCompressees(ptrDonnees, jsonSize);

    // 3. DÉCOMPRESSION des données (Étape cruciale qui manquait)
    QByteArray jsonBytes = qUncompress(donneesCompressees);

    // Vérification de sécurité pour s'assurer que la décompression a réussi
    if (jsonBytes.isEmpty()) {
      std::cerr
          << "Erreur : La décompression a échoué ou les données sont vides."
          << std::endl;
    } else {
      // 4. Lecture du JSON sur les données en clair
      QJsonDocument doc = QJsonDocument::fromJson(jsonBytes);
      QJsonObject entete = doc.object();

      sf::Vector2u vec{static_cast<uint>(entete["width"].toInt()),
                       static_cast<uint>(entete["height"].toInt())};
      handleWindow_.addProjectData(entete["scale"].toInt(), vec,
                                   entete["name"].toString().toStdString(),
                                   entete["id"].toInt());
    }
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
  case MsgProtocole::MAP_CREATE_LAYER_REP:{
    uint project_id;
    uint current_layer_id;
    uint8_t type_int;
    *(event.data_packet_) >> project_id >> current_layer_id >> type_int;
    LayerType type = static_cast<LayerType>(type_int);
    handleWindow_.createLayer(current_layer_id,type);
    break;
    }
  case MsgProtocole::MAP_REMOVE_LAYER_REP:{
    uint project_id;
    uint current_layer_id;
    *(event.data_packet_) >> project_id >> current_layer_id;
    handleWindow_.deleteLayer(current_layer_id);
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
    std::cout<<"traitement de la réponse : worker"<<std::endl;
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
    *(event.data_packet_) >> project_id >> layer_id >> asset_id >> pos_x >> pos_y >> size;
    handleWindow_.drawSprite(layer_id,asset_id,pos_x,pos_y,size);
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

  case MsgProtocole::MAP_ERASE_SPRITE_SQUARE_REP: {
    uint project_id;
    uint layer_id;
    int pos_x;
    int pos_y;
    float size;
    *(event.data_packet_) >> project_id >> layer_id >> pos_x >> pos_y >> size;
    handleWindow_.eraseSprite(layer_id, pos_x, pos_y,
                                 Shape::SQUARE,size,0);
    break;
    }
  case MsgProtocole::MAP_ERASE_SPRITE_CIRCLE_REP: {
    uint project_id;
    uint layer_id;
    int pos_x;
    int pos_y;
    float size;
    *(event.data_packet_) >> project_id >> layer_id >> pos_x >> pos_y >> size;
    handleWindow_.eraseSprite(layer_id, pos_x, pos_y,
                                 Shape::CIRCLE,size,0);
    break;
    }

  case MsgProtocole::MAP_ERASE_SPRITE_DIAM_REP: {
    uint project_id;
    uint layer_id;
    int pos_x;
    int pos_y;
    float size_x;
    float size_y;
    *(event.data_packet_) >> project_id >> layer_id >> pos_x >> pos_y >>
        size_x >> size_y;
    handleWindow_.eraseSprite(layer_id, pos_x, pos_y,
                                 Shape::DIAMOND,size_x,size_y);
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
  }
}
