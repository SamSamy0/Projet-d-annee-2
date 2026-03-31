#include "message.hpp"
#include "../../common/protocol.hpp"
#include "../reponse/reponse.hpp"
#include "../worker.hpp"




ConnectUserMessage::ConnectUserMessage(sf::Packet& dataPacket, std::shared_ptr<Client> client) {
    dataPacket >> pseudo_ >> password_;
    client_= client;
}




LoginMessage::LoginMessage(sf::Packet& dataPacket, std::shared_ptr<Client> client) 
: ConnectUserMessage(dataPacket, std::move(client)) {}

void LoginMessage::process(Worker& worker){
    uint id = worker.verifyLogin(this->pseudo_, this->password_);

  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseAuth>(std::move(client_), id);
  worker.pushNetwork(std::move(rps));
}




RegisterMessage::RegisterMessage(sf::Packet& dataPacket, std::shared_ptr<Client> client) 
: ConnectUserMessage(dataPacket, std::move(client)) {}

void RegisterMessage::process(Worker& worker) {
    uint id = worker.addUser(this->pseudo_, this->password_);

  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseAuth>(std::move(client_), id);
  worker.pushNetwork(std::move(rps));
}




CreateProjectMessage::CreateProjectMessage(sf::Packet& data_packet, std::shared_ptr<Client> client) {
    data_packet >> nomProjet_ >> size_.x >> size_.y >> scale_;
    userId_ = client->id;
    client_ = std::move(client);
}

void CreateProjectMessage::process(Worker& worker) {
    if (userId_ > 0) {
    uint idProj = worker.addProjectSQL(nomProjet_, userId_);
    client_->projectId = idProj;

    auto liveProj = LiveProject(idProj, QString::fromStdString(nomProjet_), size_.x, size_.y, scale_);
    liveProj.addConnection(userId_, 2);
    worker.mapProjet_.emplace(idProj, std::move(liveProj));

    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseCreateProject>(userId_, idProj);
    worker.pushNetwork(std::move(rps));
  }
}



RenameProjectMessage::RenameProjectMessage(sf::Packet& dataPacket, std::shared_ptr<Client>& client){
    dataPacket >> projectId_ >> newName_;
    userID_ = client->id;
}

void RenameProjectMessage::process(Worker &worker) {
  bool success = worker.renameProject(projectId_, newName_);
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseRenameProject>(userID_, projectId_, newName_,
                                               success);
  worker.pushNetwork(std::move(rps));
}

DuplicateProjectMessage::DuplicateProjectMessage(sf::Packet& dataPacket, std::shared_ptr<Client>& client){
    dataPacket >> projectId_ >> newName;
    userId_ = client->id;
}

void DuplicateProjectMessage::process(Worker& worker){
    uint newId = worker.duplicateProject(projectId_, newName, userId_);
    std::unique_ptr<Reponse>rps;
    if (newId != -1){
        rps = std::make_unique<ReponseDuplicateProject>(userId_, newId, newName);
        worker.pushNetwork(std::move(rps));
    }
}


GetProjectsListMessage::GetProjectsListMessage(sf::Packet& data_packet, std::shared_ptr<Client>& client) {
    userId_ = client->id;
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

DeleteProjectMessage::DeleteProjectMessage(sf::Packet& data_packet, std::shared_ptr<Client>& client) {
    userId_ = client->id;
    data_packet >> projectId_;
}

void DeleteProjectMessage::process(Worker &worker) {
  if (worker.getRole(userId_, projectId_) == 2) {
    worker.deleteProject(projectId_);
  }
}


GetProjectDataMessage::GetProjectDataMessage(sf::Packet& data_packet, std::shared_ptr<Client>& client) {
    userId_ = client->id;
    data_packet >> projectId_;
    client->projectId = projectId_;
}

void GetProjectDataMessage::process(Worker& worker) {
    if (worker.mapProjet_.find(projectId_) == worker.mapProjet_.end()) {
        LiveProject liveProj = LiveProject(projectId_);
        liveProj.addConnection(userId_, 1); //WARNING: LE 1 EST FORCE CODER
        worker.mapProjet_.emplace(projectId_, std::move(liveProj));

    }
    LiveProject& liveProj = worker.mapProjet_.at(projectId_);
    liveProj.addConnection(userId_, 1); //WARNING: LE 1 EST FORCE CODER
    QJsonDocument doc(liveProj.getJson());

    QByteArray jsonData = doc.toJson(QJsonDocument::Indented);

  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseProjectData>(userId_, jsonData);
  worker.pushNetwork(std::move(rps));
}


std::vector<uint> ModifProjetMessage::getUserLists(Worker& worker) {
    auto itProject = worker.mapProjet_.find(projectId_);
    
    if (itProject == worker.mapProjet_.end()) {
        return std::vector<uint>(); 
    }
    
    std::vector<uint> usersId = itProject->second.getConnected();
    auto it = std::find(usersId.begin(), usersId.end(), userId_);
    
    if (it != usersId.end()) {
        *it = usersId.back(); 
        usersId.pop_back();
    }
    std::cout<< " taille usersID : "<<usersId.size()<<std::endl;
    return usersId;
}

CreateLayerMessage::CreateLayerMessage(sf::Packet &data_packet, std::shared_ptr<Client>& client){
  userId_ = client->id;
  data_packet >>projectId_ >> calqueId_>> type_;
}

void CreateLayerMessage::process(Worker &worker){
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }



  //TODO: la condition avec liveproj
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseCreateLayer>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}


DeleteLayerMessage::DeleteLayerMessage(sf::Packet &data_packet, std::shared_ptr<Client>& client){
  userId_ = client->id;
  data_packet >>projectId_ >> calqueId_;
}

void DeleteLayerMessage::process(Worker &worker){

  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }

  //TODO: la condition avec liveproj
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseDeleteLayer>(usersId, *this);
  worker.pushNetwork(std::move(rps));

}


OrganizeLayerDawnMessage::OrganizeLayerDawnMessage(sf::Packet &data_packet, std::shared_ptr<Client>& client){
  userId_ = client->id;
  data_packet >>projectId_ >> calqueId_;
}
void OrganizeLayerDawnMessage::process(Worker &worker){
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }

  //TODO: la condition avec liveproj
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseOrganizeLayerDawn>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}


OrganizeLayerUpMessage::OrganizeLayerUpMessage(sf::Packet &data_packet, std::shared_ptr<Client>& client){
  userId_ = client->id;
  data_packet >>projectId_ >> calqueId_;
}

void OrganizeLayerUpMessage::process(Worker &worker){
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }

  //TODO: la condition avec liveproj
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseOrganizeLayerUp>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}


PutPixelsSquareMessage::PutPixelsSquareMessage(sf::Packet &data_packet,
                                             std::shared_ptr<Client>& client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_ >> red_ >>
      green_ >> blue_ >> opa_ ;

  std::cout << projectId_ << " " << calqueId_ << " " << pos_.x << " " << pos_.y << " " << taille_ << " " << red_ << " "<<
      green_ << " "<< blue_ << " "<< opa_ ;
}

void PutPixelsSquareMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }
  
  if (liveProj->second.drawPixelRect(userId_, calqueId_, pos_.x, pos_.y, taille_, red_, green_, blue_, opa_)) {
    std::cout<<"creation de la reponse"<<std::endl;
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponsePutPixelsSquare>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

PutPixelsCircleMessage::PutPixelsCircleMessage(sf::Packet& data_packet, std::shared_ptr<Client>& client) {
    userId_ = client->id;
    data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_ >>red_ >> green_ >> blue_ >> opa_ ;
}

void PutPixelsCircleMessage::process(Worker& worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }
  
  if (liveProj->second.drawPixelCircle(userId_, calqueId_, pos_.x, pos_.y, taille_, red_, green_, blue_, opa_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponsePutPixelsCircle>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

PutPixelsDiamondMessage::PutPixelsDiamondMessage(sf::Packet& data_packet, std::shared_ptr<Client>& client) {
    userId_ = client->id;
    data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> hauteur_ >> largeur_ >> red_ >> green_ >> blue_ >> opa_ ;
}

void PutPixelsDiamondMessage::process(Worker& worker) {
    auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }
  
  if (liveProj->second.drawPixelDiam(userId_, calqueId_, pos_.x, pos_.y, hauteur_, largeur_, red_, green_, blue_, opa_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponsePutPixelsDiamond>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

ErasePixelsSquareMessage::ErasePixelsSquareMessage(sf::Packet &data_packet,
                                                 std::shared_ptr<Client>& client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_;
}

void ErasePixelsSquareMessage::process(Worker &worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }
  
  if (liveProj->second.erasePixelRect(userId_, calqueId_, pos_.x, pos_.y, taille_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseErasePixelsSquare>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

ErasePixelsCircleMessage::ErasePixelsCircleMessage(sf::Packet& data_packet, std::shared_ptr<Client>& client) {
    userId_ = client->id;
    data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_;
}

void ErasePixelsCircleMessage::process(Worker& worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }
  
  if (liveProj->second.erasePixelCircle(userId_, calqueId_, pos_.x, pos_.y, taille_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseErasePixelsCircle>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}

ErasePixelsDiamondMessage::ErasePixelsDiamondMessage(sf::Packet& data_packet, std::shared_ptr<Client>& client) {
    userId_ = client->id;
    data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> hauteur_ >> largeur_;
}

void ErasePixelsDiamondMessage::process(Worker& worker) {
  auto liveProj = worker.mapProjet_.find(projectId_);

  if (liveProj == worker.mapProjet_.end()) {
    return;
  }
  
  if (liveProj->second.erasePixelDiam(userId_, calqueId_, pos_.x, pos_.y, hauteur_, largeur_)) {
    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseErasePixelsDiamond>(usersId, *this);
    worker.pushNetwork(std::move(rps));
  }
}


PutSpriteMessage::PutSpriteMessage(sf::Packet &data_packet, std::shared_ptr<Client>& client){

  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> asset_id >> pos_.x >> pos_.y >> taille_;

}
void PutSpriteMessage::process(Worker &worker){

  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponsePutSprite>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

EraseSpriteSquareMessage::EraseSpriteSquareMessage(sf::Packet &data_packet, std::shared_ptr<Client>& client){

  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_;

}

void EraseSpriteSquareMessage::process(Worker &worker){

  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponseEraseSpriteSquare>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

EraseSpriteCircleMessage::EraseSpriteCircleMessage(sf::Packet &data_packet, std::shared_ptr<Client>& client){

  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_;

}

void EraseSpriteCircleMessage::process(Worker &worker){

  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponseEraseSpriteCircle>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}


EraseSpriteDiamondMessage::EraseSpriteDiamondMessage(sf::Packet &data_packet,
                                                     std::shared_ptr<Client>& client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> hauteur_ >>
      largeur_;
}

void EraseSpriteDiamondMessage::process(Worker &worker){

  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponseEraseSpriteDiamond>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}


MoveLayerMessage::MoveLayerMessage(sf::Packet &data_packet, std::shared_ptr<Client>& client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> deltaX_ >> deltaY_;
}

void MoveLayerMessage::process(Worker &worker) {
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponseMoveLayer>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

GenerateTokenMessage::GenerateTokenMessage(sf::Packet &dataPacket,
                                           std::shared_ptr<Client>& client) {
  dataPacket >> role >> projectId_;
  userId_ = client->id;
}

void GenerateTokenMessage::process(Worker &worker) {
  std::string token = worker.generateShareToken(role, projectId_);
  std::cout << "TOKENNN" << token << std::endl;
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseGenerateToken>(userId_, token);
  worker.pushNetwork(std::move(rps));
}

CheckTokenMessage::CheckTokenMessage(sf::Packet &dataPacket, std::shared_ptr<Client>& client) {
  dataPacket >> token;
  userId_ = client->id;
}

void CheckTokenMessage::process(Worker &worker) {
  bool sucess = worker.checkShareToken(userId_, token);
  std::cout << "success or not ?" << sucess <<std::endl;
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseJoinProject>(userId_, sucess);
  worker.pushNetwork(std::move(rps));
}


DisconnectMessage::DisconnectMessage(std::shared_ptr<Client>& client) {
    userId_ = client->id;
    projectId_ = client->projectId;
}

void DisconnectMessage::process(Worker& worker) {
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseDeconnection>(userId_);
    worker.pushNetwork(std::move(rps));
    
    auto itProject = worker.mapProjet_.find(projectId_);
    
    if (itProject == worker.mapProjet_.end()) {
        return; 
    }
    
    if (itProject->second.removeConnection(userId_)) {
        std::unique_ptr<SaveTask> savetsk;
        savetsk = std::make_unique<SaveTask>(worker.mapProjet_.at(projectId_), projectId_);
        worker.pushSave(std::move(savetsk));
        //Message de sauvegarde de projet
        worker.mapProjet_.erase(projectId_);
    }

}



std::unique_ptr<IMessage> MessageFactory(sf::Packet& data_packet, std::shared_ptr<Client>& c) {
    uint8_t typeRaw;
    if (!(data_packet >> typeRaw)) return nullptr;

  MsgProtocole type = static_cast<MsgProtocole>(typeRaw);
  std::cout << "[From client " << c->id << "]:" << to_string(type) << std::endl;
  switch (type) {
  case MsgProtocole::AUTH_LOGIN_REQ:
    return std::make_unique<LoginMessage>(data_packet, std::move(c));
  case MsgProtocole::AUTH_REGISTER_REQ:
    return std::make_unique<RegisterMessage>(data_packet, std::move(c));
  case MsgProtocole::LOB_CREATE_PROJECT_REQ:
    return std::make_unique<CreateProjectMessage>(data_packet, c);

  case MsgProtocole::LOB_PROJECT_LIST_REQ:
    return std::make_unique<GetProjectsListMessage>(data_packet, c);

  case MsgProtocole::LOB_DEL_PROJECT_REQ:
    return std::make_unique<DeleteProjectMessage>(data_packet, c);

  case MsgProtocole::LOB_GET_PROJECT_DATA_REQ:
    return std::make_unique<GetProjectDataMessage>(data_packet, c);

  case MsgProtocole::LOB_RENAME_PROJECT_REQ:
    return std::make_unique<RenameProjectMessage>(data_packet, c);

  case MsgProtocole::LOB_DUPLICATE_PROJECT_REQ:
    return std::make_unique<DuplicateProjectMessage>(data_packet, c);

  case MsgProtocole::LOB_SHARE_PROJECT_REQ:
    return std::make_unique<GenerateTokenMessage>(data_packet, c);

  case MsgProtocole::LOB_JOIN_PROJECT_REQ:
    return std::make_unique<CheckTokenMessage>(data_packet, c);

  case MsgProtocole::MAP_CREATE_LAYER_REQ:
    return std::make_unique<CreateLayerMessage>(data_packet, c);

  case MsgProtocole::MAP_ORGANIZE_LAYER_UP_REQ:
    return std::make_unique<OrganizeLayerUpMessage>(data_packet,c);

  case MsgProtocole::MAP_ORGANIZE_LAYER_DAWN_REQ:
    return std::make_unique<OrganizeLayerDawnMessage>(data_packet,c);

  case MsgProtocole::MAP_REMOVE_LAYER_REQ:
    return std::make_unique<DeleteLayerMessage>(data_packet,c);
      
  case MsgProtocole::MAP_PUT_PIXELS_SQUARE_REQ:
    return std::make_unique<PutPixelsSquareMessage>(data_packet, c);

  case MsgProtocole::MAP_PUT_PIXELS_CIRCLE_REQ:
    return std::make_unique<PutPixelsCircleMessage>(data_packet, c);

  case MsgProtocole::MAP_PUT_PIXELS_DIAM_REQ:
    return std::make_unique<PutPixelsDiamondMessage>(data_packet, c);

    case MsgProtocole::MAP_ERASE_PIXELS_SQUARE_REQ:
    return std::make_unique<ErasePixelsSquareMessage>(data_packet, c);

  case MsgProtocole::MAP_ERASE_PIXELS_CIRCLE_REQ:
    return std::make_unique<ErasePixelsCircleMessage>(data_packet, c);

  case MsgProtocole::MAP_ERASE_PIXELS_DIAM_REQ:
    return std::make_unique<ErasePixelsDiamondMessage>(data_packet, c);

  case MsgProtocole::MAP_MOV_LAYER_REQ:
    return std::make_unique<MoveLayerMessage>(data_packet, c);

  case MsgProtocole::MAP_PUT_SPRITE_REQ:
    return std::make_unique<PutSpriteMessage>(data_packet, c);

  case MsgProtocole::MAP_ERASE_SPRITE_SQUARE_REQ:
    return std::make_unique<EraseSpriteSquareMessage>(data_packet, c);

  case MsgProtocole::MAP_ERASE_SPRITE_CIRCLE_REQ:
    return std::make_unique<EraseSpriteCircleMessage>(data_packet, c);

  case MsgProtocole::MAP_ERASE_SPRITE_DIAM_REQ:
    return std::make_unique<EraseSpriteDiamondMessage>(data_packet, c);
  

  default:
    std::cout << "pas de message" << std::endl;
    return nullptr;
  }  }
