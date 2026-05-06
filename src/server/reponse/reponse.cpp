#include "reponse.hpp"
#include "../../common/protocol.hpp"
#include "../servernetwork.hpp"
#include <QBuffer>
#include <QImage>
#include <QJsonDocument>
#include <SFML/Graphics/Texture.hpp>
#include "../../common/sfml_overload.hpp"

ReponseSolo::ReponseSolo(uint id) : userId_(id) {}

void ReponseSolo::envoyer(ServerNetworkManager &servManage) {
  auto client = servManage.mapUser_Socket_.find(userId_);
  if (client != servManage.mapUser_Socket_.end()) {
    client->second->sock->send(dataPacket_);
  }
}

ReponseAuth::ReponseAuth(std::shared_ptr<Client> client, uint id)
    : ReponseSolo(id), client_(std::move(client)) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::AUTH_RESULT);
  // If ID didn't generate
  if (id == -1) {
    dataPacket_ << static_cast<std::uint8_t>(0);
    // If ID generated
  } else {
    dataPacket_ << static_cast<std::uint8_t>(1) << static_cast<uint>(id);
    client_->id = id;
  }
}

ReponseGetMember::ReponseGetMember(uint userId,
                                   std::vector<MemberEntry> memberList)
    : ReponseSolo(userId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::PROJ_GET_MEMBERS_REP);
  dataPacket_ << static_cast<std::uint32_t>(memberList.size());
  for (auto &entry : memberList) {
    dataPacket_ << (entry.pseudo) << static_cast<uint32_t>(entry.role)
                << static_cast<uint32_t>(entry.userId);
  }
}

ReponseCreateProject::ReponseCreateProject(uint userId, uint projectId)
    : ReponseSolo(userId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::LOB_CREATE_PROJECT_REP);

  dataPacket_ << projectId;
}

ReponseGroupe::ReponseGroupe(std::vector<uint> usersId)
    : usersId_(std::move(usersId)) {}

void ReponseAuth::envoyer(ServerNetworkManager &servManage) {
  client_->sock->send(dataPacket_);

  if (userId_ != 0 and userId_ != -1) {
    servManage.mapUser_Socket_[userId_] = std::move(client_);
  }
}
ReponseKickUserProject::ReponseKickUserProject(std::vector<uint> usersId,
                                               uint targetId, uint projectId,
                                               bool success)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::PROJ_KICK_USER_REP);
  dataPacket_ << static_cast<std::uint8_t>(success ? 1 : 0);
  dataPacket_ << static_cast<std::uint32_t>(targetId);
  dataPacket_ << static_cast<std::uint32_t>(projectId);
}

ReponseDeconnection::ReponseDeconnection(uint id) : ReponseSolo(id) {}

void ReponseDeconnection::envoyer(ServerNetworkManager &servManager) {
  servManager.mapUser_Socket_.erase(userId_);
}

ReponseRenameProject::ReponseRenameProject(uint userId, uint projectId,
                                           std::string newName, bool success)
    : ReponseSolo(userId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::LOB_RENAME_PROJECT_REP);
  dataPacket_ << static_cast<std::uint8_t>(success ? 1 : 0);
  dataPacket_ << static_cast<std::uint32_t>(projectId);
  dataPacket_ << newName;
  ;
}

ReponseDuplicateProject::ReponseDuplicateProject(uint userId, uint projectId,
                                                 std::string newName)
    : ReponseSolo(userId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::LOB_DUPLICATE_PROJECT_REP);
  dataPacket_ << static_cast<std::uint32_t>(projectId);
  dataPacket_ << newName;
}

ReponseGenerateToken::ReponseGenerateToken(uint userId, std::string token)
    : ReponseSolo(userId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::LOB_SHARE_PROJECT_REP);
  dataPacket_ << token;
  std::cout << "TOKen in reponse: " << token << std::endl;
}
ReponseJoinProject::ReponseJoinProject(uint userId, bool success)
    : ReponseSolo(userId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::LOB_JOIN_PROJECT_REP);
  dataPacket_ << static_cast<std::uint8_t>(success ? 1 : 0);
}

ReponseProjectData::ReponseProjectData(
    uint userId, const QJsonObject &jsonDoc,
    const std::vector<uint> &layerOrder,
    const std::unordered_map<uint, QImage> &imageMap,
    const std::unordered_map<uint, SpriteLayer> &spriteMap,
    const QJsonArray &chat, const std::map<uint, sf::Texture> &textureMap)
    : ReponseSolo(userId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::LOB_GET_PROJECT_DATA_REP);

  QJsonDocument doc(jsonDoc);
  QByteArray jsonData = doc.toJson(QJsonDocument::Indented);
  QByteArray jsonCompresse = qCompress(jsonData, 9);
  dataPacket_ << static_cast<std::uint32_t>(jsonCompresse.size());
  dataPacket_.append(jsonCompresse.constData(), jsonCompresse.size());

  doc = QJsonDocument(chat);
  QByteArray chatData = doc.toJson(QJsonDocument::Indented);
  QByteArray chatCompress = qCompress(chatData, 9);
  dataPacket_ << static_cast<std::uint32_t>(chatCompress.size());
  dataPacket_.append(chatCompress.constData(), chatCompress.size());

  dataPacket_ << static_cast<std::uint32_t>(textureMap.size());
  for (const auto &pair : textureMap) {
    dataPacket_ << static_cast<std::uint32_t>(pair.first);
    sf::Image image = pair.second.copyToImage();
    uint32_t width = image.getSize().x;
    uint32_t height = image.getSize().y;

    dataPacket_ << width << height;

    const uint8_t* pixels = image.getPixelsPtr();
    dataPacket_.append(pixels, width * height * 4);
  }
  
  for (uint id : layerOrder) {
    dataPacket_ << id;
    if (spriteMap.find(id) != spriteMap.end()) {
      dataPacket_ << (uint8_t)1;
      QJsonDocument doc(spriteMap.at(id).load());
      QByteArray layerData = doc.toJson(QJsonDocument::Indented);
      QByteArray layerCompress = qCompress(layerData);
      dataPacket_ << static_cast<std::uint32_t>(layerCompress.size());
      dataPacket_.append(layerCompress.constData(), layerCompress.size());
    } else if (imageMap.find(id) != imageMap.end()) {
      dataPacket_ << (uint8_t)0;
      const QImage &img = imageMap.at(id);
      QByteArray imageBytes = imageToBytes(img);
      dataPacket_ << static_cast<std::uint32_t>(imageBytes.size());
      dataPacket_.append(imageBytes.constData(), imageBytes.size());
    }
  }
}

QByteArray ReponseProjectData::imageToBytes(const QImage &image) {
  QByteArray byteArray;
  QBuffer buffer(&byteArray);
  buffer.open(QIODevice::WriteOnly);
  image.save(&buffer, "PNG"); // Le format PNG compresse l'image proprement
  return byteArray;
}

ReponseUsersProjects::ReponseUsersProjects(uint userId,
                                           std::vector<ProjectEntry> &projects)
    : ReponseSolo(userId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::LOB_PROJECT_LIST_REP);

  dataPacket_ << static_cast<std::uint32_t>(projects.size());

  for (const auto &entry : projects) {
    dataPacket_ << static_cast<uint32_t>(entry.projectId) << entry.name
                << entry.role;
  }
}

ReponseChangeRole::ReponseChangeRole(std::vector<uint> usersId, uint target,
                                     uint projectId, int8_t role, bool success)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::PROJ_CHANGE_ROLE_REP);
  dataPacket_ << static_cast<std::uint8_t>(success ? 1 : 0);

  if (success) {
    dataPacket_ << projectId;
    dataPacket_ << target;
    dataPacket_ << role;
  }
}

void ReponseGroupe::envoyer(ServerNetworkManager &servManager) {
  for (auto Id : usersId_) {
    auto client = servManager.mapUser_Socket_.find(Id);
    if (client != servManager.mapUser_Socket_.end()) {
      client->second->sock->send(dataPacket_);
    }
  }
}

ReponsePutPixelsSquare::ReponsePutPixelsSquare(std::vector<uint> usersId,
                                               PutPixelsSquareMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::MAP_PUT_PIXELS_SQUARE_REP);
  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.pos_.x
              << mess.pos_.y;
  dataPacket_ << mess.taille_ << mess.red_ << mess.green_ << mess.blue_
              << mess.opa_;
}

ReponseCreateLayer::ReponseCreateLayer(std::vector<uint> usersId,
                                       CreateLayerMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_CREATE_LAYER_REP);
  dataPacket_ << mess.projectId_ << mess.type_;
}

ReponseDeleteLayer::ReponseDeleteLayer(std::vector<uint> usersId,
                                       DeleteLayerMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_REMOVE_LAYER_REP);
  dataPacket_ << mess.projectId_ << mess.calqueId_;
}

ReponseRenameLayer::ReponseRenameLayer(std::vector<uint> usersId,
                                       RenameLayerMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_RENAME_LAYER_REP);
  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.name_;
}

ReponseOrganizeLayerUp::ReponseOrganizeLayerUp(std::vector<uint> usersId,
                                               OrganizeLayerUpMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::MAP_ORGANIZE_LAYER_UP_REP);
  dataPacket_ << mess.projectId_ << mess.calqueId_;
}

ReponseOrganizeLayerDown::ReponseOrganizeLayerDown(
    std::vector<uint> usersId, OrganizeLayerDownMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::MAP_ORGANIZE_LAYER_DOWN_REP);
  dataPacket_ << mess.projectId_ << mess.calqueId_;
}

ReponsePutPixelsCircle::ReponsePutPixelsCircle(std::vector<uint> usersId,
                                               PutPixelsCircleMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::MAP_PUT_PIXELS_CIRCLE_REP);

  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.pos_.x
              << mess.pos_.y;
  dataPacket_ << mess.taille_ << mess.red_ << mess.green_ << mess.blue_
              << mess.opa_;
}

ReponsePutPixelsDiamond::ReponsePutPixelsDiamond(std::vector<uint> usersId,
                                                 PutPixelsDiamondMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::MAP_PUT_PIXELS_DIAM_REP);
  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.pos_.x
              << mess.pos_.y;
  dataPacket_ << mess.hauteur_ << mess.largeur_ << mess.red_ << mess.green_
              << mess.blue_ << mess.opa_;
}

ReponseErasePixelsSquare::ReponseErasePixelsSquare(
    std::vector<uint> usersId, ErasePixelsSquareMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::MAP_ERASE_PIXELS_SQUARE_REP);
  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.pos_.x
              << mess.pos_.y;
  dataPacket_ << mess.taille_;
}

ReponseErasePixelsCircle::ReponseErasePixelsCircle(
    std::vector<uint> usersId, ErasePixelsCircleMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::MAP_ERASE_PIXELS_CIRCLE_REP);
  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.pos_.x
              << mess.pos_.y;
  dataPacket_ << mess.taille_;
}

ReponseErasePixelsDiamond::ReponseErasePixelsDiamond(
    std::vector<uint> usersId, ErasePixelsDiamondMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::MAP_ERASE_PIXELS_DIAM_REP);

  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.pos_.x
              << mess.pos_.y;
  dataPacket_ << mess.hauteur_ << mess.largeur_;
}

ReponsePutSprite::ReponsePutSprite(std::vector<uint> usersId,
                                   PutSpriteMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_PUT_SPRITE_REP);

  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.asset_id_
              << mess.pos_.x << mess.pos_.y;
  dataPacket_ << mess.taille_;
}

ReponseEraseSprite::ReponseEraseSprite(std::vector<uint> usersId,
                                       EraseSpriteMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_ERASE_SPRITE_REP);

  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.sprite_id_;
}

ReponseMoveSprite::ReponseMoveSprite(std::vector<uint> usersId,
                                     MoveSpriteMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_MOV_SPRITE_REP);

  dataPacket_ << mess.projectId_ << mess.calqueId_
              << static_cast<uint>(mess.sprite_ids_.size());
  dataPacket_ << mess.x_ << mess.y_;
  for (uint id : mess.sprite_ids_) {
    dataPacket_ << id;
  }
}

ReponseResizeSprite::ReponseResizeSprite(std::vector<uint> usersId,
                                         ResizeSpriteMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_RESIZE_SPRITE_REP);

  dataPacket_ << mess.projectId_ << mess.calqueId_
              << static_cast<uint>(mess.sprite_ids_.size());
  dataPacket_ << mess.scale_;
  for (size_t i = 0; i < mess.sprite_ids_.size(); ++i) {
    dataPacket_ << mess.sprite_ids_[i] << mess.x_[i] << mess.y_[i];
  }
}

ReponseRotateSprite::ReponseRotateSprite(std::vector<uint> usersId,
                                         RotateSpriteMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_ROTATE_SPRITE_REP);

  dataPacket_ << mess.projectId_ << mess.calqueId_
              << static_cast<uint>(mess.sprite_ids_.size());
  dataPacket_ << mess.angle_;
  for (size_t i = 0; i < mess.sprite_ids_.size(); ++i) {
    dataPacket_ << mess.sprite_ids_[i] << mess.x_[i] << mess.y_[i];
  }
}

ReponseMoveLayer::ReponseMoveLayer(std::vector<uint> usersId,
                                   MoveLayerMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_MOV_LAYER_REP);

  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.deltaX_
              << mess.deltaY_;
}



ReponseAutoFill::ReponseAutoFill(std::vector<uint> usersId, AutoFillMessage& mess) : ReponseGroupe(usersId){
    dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_AUTOFILL_REP);

    dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.count_ << mess.rotation_ << mess.size_;
    for(std::string id : mess.asset_ids_){
        dataPacket_ << id;
    }
    for(sf::Vector2f pos : mess.positions_){
        dataPacket_ << pos.x << pos.y;
    }
}

ReponseChat::ReponseChat(std::vector<uint> usersId, ChatMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::CHAT_MESSAGE_REP);
  dataPacket_ << mess.pseudo_ << mess.message_ << mess.min_ << mess.hour_
              << mess.day_ << mess.month_ << mess.year_;
}

ReponseChatSystem::ReponseChatSystem(std::vector<uint> usersId, SystemNotification &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::CHAT_SYSTEME_REP);

  dataPacket_ << mess.getAuthor().getUser() << static_cast<uint8_t>(mess.getTypeNotif()) << mess.getDate().min_
              << mess.getDate().hour_ << mess.getDate().day_
              << mess.getDate().month_ << mess.getDate().year_;
}

ReponseExport::ReponseExport(uint userId, std::string projectName,
                             QByteArray data)
    : ReponseSolo(userId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::LOB_EXPORT_NATIVE_PROJECT_REP);
  QByteArray dataCompress = qCompress(data, 9);
  dataPacket_ << static_cast<std::uint32_t>(dataCompress.size());
  dataPacket_ << (projectName);
  dataPacket_.append(dataCompress.constData(), dataCompress.size());
  std::cout << "Taille du projet compressé dans la réponse : "
            << dataCompress.size() << " octets" << std::endl;
}

ReponseAddSprite::ReponseAddSprite(std::vector<uint> usersId, AddSpriteMessage &mess) : ReponseGroupe(usersId){
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_ADD_SPRITE_REP);
  dataPacket_ << mess.spriteId_;
  dataPacket_ << mess.sprite_;

}
