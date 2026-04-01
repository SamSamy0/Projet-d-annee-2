#include "reponse.hpp"
#include "../../common/protocol.hpp"
#include "../servernetwork.hpp"

ReponseSolo::ReponseSolo(uint id) : userId_(id) {}

void ReponseSolo::envoyer(ServerNetworkManager &servManage) {
  auto client = servManage.map_.find(userId_);
  if (client != servManage.map_.end()) {
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

void ReponseAuth::envoyer(ServerNetworkManager &servManage) {
  client_->sock->send(dataPacket_);

  if (userId_ != 0) {
    servManage.map_[userId_] = std::move(client_);
  }
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
}
ReponseJoinProject::ReponseJoinProject(uint userId, bool success)
    : ReponseSolo(userId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::LOB_JOIN_PROJECT_REP);
  dataPacket_ << static_cast<std::uint8_t>(success ? 1 : 0);
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

ReponseProjectData::ReponseProjectData(uint userId, QByteArray &jsonData)
    : ReponseSolo(userId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::LOB_GET_PROJECT_DATA_REP);

  QByteArray jsonCompresse = qCompress(jsonData, 9);
  dataPacket_ << static_cast<std::uint32_t>(jsonCompresse.size());
  dataPacket_.append(jsonCompresse.constData(), jsonCompresse.size());
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

ReponseCreateProject::ReponseCreateProject(uint userId, uint projectId)
    : ReponseSolo(userId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::LOB_CREATE_PROJECT_REP);

  dataPacket_ << projectId;
}

ReponseGroupe::ReponseGroupe(std::vector<uint> usersId)
    : usersId_(std::move(usersId)) {}

void ReponseGroupe::envoyer(ServerNetworkManager &servManager) {
  for (auto Id : usersId_) {
    auto client = servManager.map_.find(Id);
    if (client != servManager.map_.end()) {
      client->second->sock->send(dataPacket_);
    }
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

ReponsePutPixelsCircle::ReponsePutPixelsCircle(std::vector<uint> usersId,
                                               PutPixelsCircleMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::MAP_PUT_PIXELS_CIRCLE_REP);

  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.pos_.x
              << mess.pos_.y;
  dataPacket_ << mess.red_ << mess.green_ << mess.blue_ << mess.opa_
              << mess.taille_;
}

ReponsePutPixelsSquare::ReponsePutPixelsSquare(std::vector<uint> usersId,
                                               PutPixelsSquareMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::MAP_PUT_PIXELS_SQUARE_REP);

  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.pos_.x
              << mess.pos_.y;
  dataPacket_ << mess.red_ << mess.green_ << mess.blue_ << mess.opa_
              << mess.taille_;
}

ReponsePutPixelsDiamond::ReponsePutPixelsDiamond(std::vector<uint> usersId,
                                                 PutPixelsDiamondMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::MAP_PUT_PIXELS_DIAM_REP);

  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.pos_.x
              << mess.pos_.y;
  dataPacket_ << mess.red_ << mess.green_ << mess.blue_ << mess.opa_
              << mess.hauteur_ << mess.largeur_;
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

ReponseErasePixelsSquare::ReponseErasePixelsSquare(
    std::vector<uint> usersId, ErasePixelsSquareMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::MAP_ERASE_PIXELS_SQUARE_REP);

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

  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.asset_id
              << mess.pos_.x << mess.pos_.y;
  dataPacket_ << mess.taille_;
}

ReponseEraseSpriteSquare::ReponseEraseSpriteSquare(
    std::vector<uint> usersId, EraseSpriteSquareMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::MAP_ERASE_SPRITE_SQUARE_REP);

  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.pos_.x
              << mess.pos_.y;
  dataPacket_ << mess.taille_;
}

ReponseEraseSpriteCircle::ReponseEraseSpriteCircle(
    std::vector<uint> usersId, EraseSpriteCircleMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::MAP_ERASE_SPRITE_CIRCLE_REP);

  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.pos_.x
              << mess.pos_.y;
  dataPacket_ << mess.taille_;
}

ReponseEraseSpriteDiamond::ReponseEraseSpriteDiamond(
    std::vector<uint> usersId, EraseSpriteDiamondMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(
      MsgProtocole::MAP_ERASE_SPRITE_CIRCLE_REP);

  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.pos_.x
              << mess.pos_.y;
  dataPacket_ << mess.hauteur_ << mess.largeur_;
}

ReponseMoveLayer::ReponseMoveLayer(std::vector<uint> usersId,
                                   MoveLayerMessage &mess)
    : ReponseGroupe(usersId) {
  dataPacket_ << static_cast<std::uint8_t>(MsgProtocole::MAP_MOV_LAYER_REP);

  dataPacket_ << mess.projectId_ << mess.calqueId_ << mess.deltaX_
              << mess.deltaY_;
}
