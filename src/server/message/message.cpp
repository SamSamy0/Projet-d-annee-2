#include "message.hpp"
#include "../../common/protocol.hpp"
#include "../reponse/reponse.hpp"
#include "../worker.hpp"

LoginMessage::LoginMessage(sf::Packet &dataPacket,
                           std::shared_ptr<Client> client) {
  dataPacket >> pseudo_ >> password_;
  client_ = client;
}

void LoginMessage::process(Worker &worker) {
  uint id = worker.verifyLogin(this->pseudo_, this->password_);

  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseAuth>(std::move(client_), id);
  worker.pushNetwork(std::move(rps));
}

RegisterMessage::RegisterMessage(sf::Packet &dataPacket,
                                 std::shared_ptr<Client> client) {
  dataPacket >> pseudo_ >> password_;
  client_ = client;
}

void RegisterMessage::process(Worker &worker) {
  uint id = worker.addUser(this->pseudo_, this->password_);

  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseAuth>(std::move(client_), id);
  worker.pushNetwork(std::move(rps));
}

CreateProjectMessage::CreateProjectMessage(sf::Packet &data_packet,
                                           uint userId) {
  data_packet >> nomProjet_ >> size_.x >> size_.y >> scale_;
  userId_ = userId;
}

void CreateProjectMessage::process(Worker &worker) {
  if (userId_ > 0) {
    uint idProj = worker.addProjectSQL(nomProjet_, userId_);
    worker.createProjectJson(idProj, nomProjet_, size_.x, size_.y, scale_);

    auto liveProj = LiveProject(worker.loadProjectJson(idProj));
    liveProj.connectedID_.push_back(userId_);
    liveProj.userRole_[userId_] = worker.getRole(userId_, idProj);
    worker.mapProjet_.emplace(idProj, std::move(liveProj));

    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseCreateProject>(userId_, idProj);
    worker.pushNetwork(std::move(rps));
  }
}

RenameProjectMessage::RenameProjectMessage(sf::Packet &dataPacket,
                                           uint userId) {
  dataPacket >> projectId_ >> newName_;
  userID_ = userId;
}

void RenameProjectMessage::process(Worker &worker) {
  bool success = worker.renameProject(projectId_, newName_);

  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseRenameProject>(userID_, projectId_, newName_,
                                               success);
  worker.pushNetwork(std::move(rps));
}

DuplicateProjectMessage::DuplicateProjectMessage(sf::Packet &dataPacket,
                                                 uint userId) {
  dataPacket >> projectId_ >> newName;
  userId_ = userId;
}

void DuplicateProjectMessage::process(Worker &worker) {
  long long newId = worker.duplicateProject(projectId_, newName, userId_);
  std::unique_ptr<Reponse> rps;
  if (newId != -1) {
    rps = std::make_unique<ReponseDuplicateProject>(userId_, newId, newName);
    worker.pushNetwork(std::move(rps));
  }
}

GetProjectsListMessage::GetProjectsListMessage(sf::Packet &data_packet,
                                               uint userId) {
  userId_ = userId;
}

void GetProjectsListMessage::process(Worker &worker) {
  std::vector<ProjectEntry> projects;
  if (userId_ != 0) {
    projects = worker.getUserProjects(userId_);
  }

  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseUsersProjects>(userId_, projects);
  worker.pushNetwork(std::move(rps));
}

DeleteProjectMessage::DeleteProjectMessage(sf::Packet &data_packet,
                                           uint userId) {
  userId_ = userId;
  data_packet >> projectId_;
}

void DeleteProjectMessage::process(Worker &worker) {
  if (worker.getRole(userId_, projectId_) == 2) {
    // if owner -> delete project for everybody
    worker.deleteProject(projectId_);
  } else {
    worker.removeLink(userId_, projectId_);
  }
}

LeaveProjectMessage::LeaveProjectMessage(sf::Packet &data_packet, uint userId) {
  userId_ = userId;
  data_packet >> projectId_;
}
void LeaveProjectMessage::process(Worker &worker) {
  worker.removeLink(userId_, projectId_);
}

KickUserMessage::KickUserMessage(sf::Packet &data_packet, uint userId) {
  userId_ = userId;
  data_packet >> targetId_ >> projectId_;
}
void KickUserMessage::process(Worker &worker) {
  bool success = worker.removeLink(targetId_, projectId_);
  std::unique_ptr<Reponse> rps;
  std::cout << "Reussi a kick( message): " << success << std::endl;
  rps =
      std::make_unique<ReponseKickUserProject>(targetId_, projectId_, success);
  worker.pushNetwork(std::move(rps));
}

GetMemberMessage::GetMemberMessage(sf::Packet &data_packet, uint userId) {
  userId_ = userId;
  data_packet >> projectId_;
}

void GetMemberMessage::process(Worker &worker) {
  std::vector<MemberEntry> memberList = worker.getProjectMembers(projectId_);

  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseGetMember>(userId_, memberList);
  worker.pushNetwork(std::move(rps));
}

GetProjectDataMessage::GetProjectDataMessage(sf::Packet &data_packet,
                                             uint userId) {
  userId_ = userId;
  data_packet >> projectId_;
}

void GetProjectDataMessage::process(Worker &worker) {
  if (worker.mapProjet_.find(projectId_) == worker.mapProjet_.end()) {
    auto liveProj = LiveProject(worker.loadProjectJson(projectId_));
    liveProj.connectedID_.push_back(userId_);
    liveProj.userRole_[userId_] = worker.getRole(userId_, projectId_);
    worker.mapProjet_.emplace(projectId_, std::move(liveProj));

  } else {
    auto item_id_Projet = (worker.mapProjet_).find(projectId_);
    worker.writeProjetJson(item_id_Projet->second.json_, projectId_);
    item_id_Projet->second.connectedID_.push_back(userId_);
    item_id_Projet->second.userRole_[userId_] =
        worker.getRole(userId_, projectId_);
  }
  QByteArray jsonData = worker.getByteJson(projectId_);

  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseProjectData>(userId_, jsonData);
  worker.pushNetwork(std::move(rps));
}

std::vector<uint> ModifProjetMessage::getUserLists(Worker &worker) {
  std::vector<uint> usersId =
      worker.mapProjet_.find(projectId_)->second.connectedID_;

  auto it = std::find(usersId.begin(), usersId.end(), userId_);
  if (it != usersId.end()) {
    *it = usersId.back();
    usersId.pop_back();
  }
  return usersId;
}
ChangeRoleMessage::ChangeRoleMessage(sf::Packet &data_packet, uint userId) {
  userId_ = userId;
  data_packet >> target_ >> projectId_ >> role_;
}

void ChangeRoleMessage::process(Worker &worker) {
  bool success = worker.changeRole(target_, projectId_, role_);

  // Getting all connected users
  std::vector<uint> usersId;
  auto it = worker.mapProjet_.find(projectId_);
  if (it != worker.mapProjet_.end()) {
    usersId = it->second.connectedID_;
  }

  // Building the group response
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseChangeRole>(usersId, target_, projectId_, role_,
                                            success);
  worker.pushNetwork(std::move(rps));
}

PutPixelsSquareMessage::PutPixelsSquareMessage(sf::Packet &data_packet,
                                               uint userId) {
  userId_ = userId;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> red_ >>
      green_ >> blue_ >> opa_ >> taille_;
}

void PutPixelsSquareMessage::process(Worker &worker) {
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponsePutPixelsSquare>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

PutPixelsCircleMessage::PutPixelsCircleMessage(sf::Packet &data_packet,
                                               uint userId) {
  userId_ = userId;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> red_ >>
      green_ >> blue_ >> opa_ >> taille_;
}

void PutPixelsCircleMessage::process(Worker &worker) {
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponsePutPixelsCircle>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

PutPixelsDiamondMessage::PutPixelsDiamondMessage(sf::Packet &data_packet,
                                                 uint userId) {
  userId_ = userId;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> red_ >>
      green_ >> blue_ >> opa_ >> hauteur_ >> largeur_;
}

void PutPixelsDiamondMessage::process(Worker &worker) {
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponsePutPixelsDiamond>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

ErasePixelsSquareMessage::ErasePixelsSquareMessage(sf::Packet &data_packet,
                                                   uint userId) {
  userId_ = userId;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_;
}

void ErasePixelsSquareMessage::process(Worker &worker) {
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponseErasePixelsSquare>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

ErasePixelsCircleMessage::ErasePixelsCircleMessage(sf::Packet &data_packet,
                                                   uint userId) {
  userId_ = userId;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_;
}

void ErasePixelsCircleMessage::process(Worker &worker) {
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponseErasePixelsCircle>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

ErasePixelsDiamondMessage::ErasePixelsDiamondMessage(sf::Packet &data_packet,
                                                     uint userId) {
  userId_ = userId;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> hauteur_ >>
      largeur_;
}

void ErasePixelsDiamondMessage::process(Worker &worker) {
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponseErasePixelsDiamond>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

PutSpriteMessage::PutSpriteMessage(sf::Packet &data_packet, uint userId) {
  userId_ = userId;
  data_packet >> projectId_ >> calqueId_ >> asset_id >> pos_.x >> pos_.y >>
      taille_;
}
void PutSpriteMessage::process(Worker &worker) {
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponsePutSprite>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

EraseSpriteSquareMessage::EraseSpriteSquareMessage(sf::Packet &data_packet,
                                                   uint userId) {
  userId_ = userId;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_;
}

void EraseSpriteSquareMessage::process(Worker &worker) {
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponseEraseSpriteSquare>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

EraseSpriteCircleMessage::EraseSpriteCircleMessage(sf::Packet &data_packet,
                                                   uint userId) {
  userId_ = userId;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_;
}

void EraseSpriteCircleMessage::process(Worker &worker) {
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponseEraseSpriteCircle>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

EraseSpriteDiamondMessage::EraseSpriteDiamondMessage(sf::Packet &data_packet,
                                                     uint userId) {
  userId_ = userId;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> hauteur_ >>
      largeur_;
}

void EraseSpriteDiamondMessage::process(Worker &worker) {
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponseEraseSpriteDiamond>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

MoveLayerMessage::MoveLayerMessage(sf::Packet &data_packet, uint userId) {
  userId_ = userId;
  data_packet >> projectId_ >> calqueId_ >> deltaX_ >> deltaY_;
}

void MoveLayerMessage::process(Worker &worker) {
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponseMoveLayer>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

GenerateTokenMessage::GenerateTokenMessage(sf::Packet &dataPacket,
                                           uint userId) {
  dataPacket >> role >> projectId_;
  userId_ = userId;
}

void GenerateTokenMessage::process(Worker &worker) {
  std::string token = worker.generateShareToken(role, projectId_);
  std::cout << "TOKENNN" << token << std::endl;
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseGenerateToken>(userId_, token);
  worker.pushNetwork(std::move(rps));
}

CheckTokenMessage::CheckTokenMessage(sf::Packet &dataPacket, uint userId) {
  dataPacket >> token;
  userId_ = userId;
}

void CheckTokenMessage::process(Worker &worker) {
  bool sucess = worker.checkShareToken(userId_, token);
  std::cout << "success or not ?" << sucess << std::endl;
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseJoinProject>(userId_, sucess);
  worker.pushNetwork(std::move(rps));
}

std::unique_ptr<IMessage> MessageFactory(sf::Packet &data_packet,
                                         std::shared_ptr<Client> c) {
  uint8_t typeRaw;
  if (!(data_packet >> typeRaw))
    return nullptr;

  MsgProtocole type = static_cast<MsgProtocole>(typeRaw);
  std::cout << "[From client " << c->id << "]:" << to_string(type) << std::endl;
  switch (type) {
  case MsgProtocole::AUTH_LOGIN_REQ:
    return std::make_unique<LoginMessage>(data_packet, std::move(c));

  case MsgProtocole::AUTH_REGISTER_REQ:
    return std::make_unique<RegisterMessage>(data_packet, std::move(c));
  case MsgProtocole::LOB_CREATE_PROJECT_REQ:
    return std::make_unique<CreateProjectMessage>(data_packet, c->id);

  case MsgProtocole::LOB_PROJECT_LIST_REQ:
    return std::make_unique<GetProjectsListMessage>(data_packet, c->id);

  case MsgProtocole::LOB_DEL_PROJECT_REQ:
    return std::make_unique<DeleteProjectMessage>(data_packet, c->id);

  case MsgProtocole::LOB_GET_PROJECT_DATA_REQ:
    return std::make_unique<GetProjectDataMessage>(data_packet, c->id);

  case MsgProtocole::LOB_RENAME_PROJECT_REQ:
    return std::make_unique<RenameProjectMessage>(data_packet, c->id);

  case MsgProtocole::LOB_DUPLICATE_PROJECT_REQ:
    return std::make_unique<DuplicateProjectMessage>(data_packet, c->id);

  case MsgProtocole::PROJ_GET_MEMBERS_REQ:
    return std::make_unique<GetMemberMessage>(data_packet, c->id);

  case MsgProtocole::LOB_SHARE_PROJECT_REQ:
    return std::make_unique<GenerateTokenMessage>(data_packet, c->id);

  case MsgProtocole::LOB_JOIN_PROJECT_REQ:
    return std::make_unique<CheckTokenMessage>(data_packet, c->id);

  case MsgProtocole::MAP_PUT_PIXELS_SQUARE_REQ:
    return std::make_unique<PutPixelsSquareMessage>(data_packet, c->id);

  case MsgProtocole::MAP_PUT_PIXELS_CIRCLE_REQ:
    return std::make_unique<PutPixelsCircleMessage>(data_packet, c->id);

  case MsgProtocole::MAP_PUT_PIXELS_DIAM_REQ:
    return std::make_unique<PutPixelsDiamondMessage>(data_packet, c->id);

  case MsgProtocole::MAP_MOV_LAYER_REQ:
    return std::make_unique<MoveLayerMessage>(data_packet, c->id);

  case MsgProtocole::MAP_PUT_SPRITE_REQ:
    return std::make_unique<PutSpriteMessage>(data_packet, c->id);

  case MsgProtocole::MAP_ERASE_SPRITE_SQUARE_REQ:
    return std::make_unique<EraseSpriteSquareMessage>(data_packet, c->id);

  case MsgProtocole::MAP_ERASE_SPRITE_CIRCLE_REQ:
    return std::make_unique<EraseSpriteCircleMessage>(data_packet, c->id);

  case MsgProtocole::MAP_ERASE_SPRITE_DIAM_REQ:
    return std::make_unique<EraseSpriteDiamondMessage>(data_packet, c->id);

  case MsgProtocole::PROJ_CHANGE_ROLE_REQ:
    return std::make_unique<ChangeRoleMessage>(data_packet, c->id);

  case MsgProtocole::PROJ_LEAVE_PROJ_REQ:
    return std::make_unique<LeaveProjectMessage>(data_packet, c->id);

  case MsgProtocole::PROJ_KICK_USER_REQ:
    return std::make_unique<KickUserMessage>(data_packet, c->id);

  default:
    std::cout << "pas de message" << std::endl;
    return nullptr;
  }
}
