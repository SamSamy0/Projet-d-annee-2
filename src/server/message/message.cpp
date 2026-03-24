#include "message.hpp"
#include "../../common/protocol.hpp"
#include "../reponse/reponse.hpp"
#include "../worker.hpp"
#include <QPainter>
#include <QImage>



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
        auto liveProj = LiveProject(worker.loadProjectJson(projectId_));
        liveProj.addConnection(userId_, worker.getRole(userId_, projectId_));
        for(auto it = liveProj.layersImage_.begin(); it != liveProj.layersImage_.end(); ++it) {
            liveProj.addLayerImage(it->first, worker.getProjMngr().loadImage(projectId_, it->first));
        }
        worker.mapProjet_.emplace(projectId_, std::move(liveProj));

    }
    LiveProject& liveProj = worker.mapProjet_.at(projectId_);
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
    return usersId;
}

PutPixelsSquareMessage::PutPixelsSquareMessage(sf::Packet &data_packet,
                                             std::shared_ptr<Client>& client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> red_ >>
      green_ >> blue_ >> opa_ >> taille_;
}

void PutPixelsSquareMessage::process(Worker &worker) {
    uint scale = worker.mapProjet_.at(projectId_).getScale();
    QImage & image = worker.mapProjet_.at(projectId_).layersImage_[calqueId_];
    QPainter painter(&image);
    QColor color(red_, green_, blue_, opa_);

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(color));
    
    float topLeftX = pos_.x - (taille_*scale)/2.0f;
    float topLeftY = pos_.y - (taille_*scale)/2.0f;

    painter.drawRect(QRectF(topLeftX, topLeftY, taille_ * scale, taille_ * scale));
  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponsePutPixelsSquare>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

PutPixelsCircleMessage::PutPixelsCircleMessage(sf::Packet& data_packet, std::shared_ptr<Client>& client) {
    userId_ = client->id;
    data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> red_ >> green_ >> blue_ >> opa_ >> taille_;
}

void PutPixelsCircleMessage::process(Worker& worker) {
    uint scale = worker.mapProjet_.at(projectId_).getScale();
    QImage & image = worker.mapProjet_.at(projectId_).layersImage_[calqueId_];
    QPainter painter(&image);
    QColor color(red_, green_, blue_, opa_);
    QPolygonF polygon;

    for (int i = 0; i < 30; ++i) {

        float angle = i * 2 * M_PI / 30;
        float px = pos_.x + std::cos(angle) * taille_ * scale/ 2.0f;
        float py = pos_.y + std::sin(angle) * taille_ * scale / 2.0f;
        polygon << QPointF(px, py);
    }

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(color));

    painter.drawPolygon(polygon);

    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponsePutPixelsCircle>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

PutPixelsDiamondMessage::PutPixelsDiamondMessage(sf::Packet& data_packet, std::shared_ptr<Client>& client) {
    userId_ = client->id;
    data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> red_ >> green_ >> blue_ >> opa_ >> hauteur_ >> largeur_;
}

void PutPixelsDiamondMessage::process(Worker& worker) {
    uint scale = worker.mapProjet_.at(projectId_).getScale();
    QImage & image = worker.mapProjet_.at(projectId_).layersImage_[calqueId_];
    QPainter painter(&image);
    QColor color(red_, green_, blue_, opa_);
    QPolygonF polygon;

    float h_demi = hauteur_ * scale/ 2.0f;
    float l_demi = largeur_ * scale / 2.0f;
    polygon << QPointF(pos_.x, pos_.y - h_demi);
    polygon << QPointF(pos_.x + l_demi, pos_.y);
    polygon << QPointF(pos_.x, pos_.y + h_demi);
    polygon << QPointF(pos_.x - l_demi, pos_.y);

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(color));

    painter.drawPolygon(polygon);


    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponsePutPixelsDiamond>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

ErasePixelsSquareMessage::ErasePixelsSquareMessage(sf::Packet &data_packet,
                                                 std::shared_ptr<Client>& client) {
  userId_ = client->id;
  data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_;
}

void ErasePixelsSquareMessage::process(Worker &worker) {
    uint scale = worker.mapProjet_.at(projectId_).getScale();
    QImage & image = worker.mapProjet_.at(projectId_).layersImage_[calqueId_];
    QPainter painter(&image);

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::transparent));
    
    float topLeftX = pos_.x - (taille_*scale)/2.0f;
    float topLeftY = pos_.y - (taille_*scale)/2.0f;

    painter.drawRect(QRectF(topLeftX, topLeftY, taille_ * scale, taille_ * scale));  std::vector<uint> usersId = this->getUserLists(worker);
  std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponseErasePixelsSquare>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

ErasePixelsCircleMessage::ErasePixelsCircleMessage(sf::Packet& data_packet, std::shared_ptr<Client>& client) {
    userId_ = client->id;
    data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_;
}

void ErasePixelsCircleMessage::process(Worker& worker) {
    uint scale = worker.mapProjet_.at(projectId_).getScale();
    QImage & image = worker.mapProjet_.at(projectId_).layersImage_[calqueId_];
    QPainter painter(&image);
    QPolygonF polygon;

    for (int i = 0; i < 30; ++i) {

        float angle = i * 2 * M_PI / 30;
        float px = pos_.x + std::cos(angle) * taille_ * scale/ 2.0f;
        float py = pos_.y + std::sin(angle) * taille_ * scale / 2.0f;
        polygon << QPointF(px, py);
    }

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::transparent));

    painter.drawPolygon(polygon);

    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponseErasePixelsCircle>(usersId, *this);
  worker.pushNetwork(std::move(rps));
}

ErasePixelsDiamondMessage::ErasePixelsDiamondMessage(sf::Packet& data_packet, std::shared_ptr<Client>& client) {
    userId_ = client->id;
    data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> hauteur_ >> largeur_;
}

void ErasePixelsDiamondMessage::process(Worker& worker) {
    uint scale = worker.mapProjet_.at(projectId_).getScale();
    QImage & image = worker.mapProjet_.at(projectId_).layersImage_[calqueId_];
    QPainter painter(&image);
    QPolygonF polygon;

    float h_demi = hauteur_ * scale/ 2.0f;
    float l_demi = largeur_ * scale / 2.0f;
    polygon << QPointF(pos_.x, pos_.y - h_demi);
    polygon << QPointF(pos_.x + l_demi, pos_.y);
    polygon << QPointF(pos_.x, pos_.y + h_demi);
    polygon << QPointF(pos_.x - l_demi, pos_.y);

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::transparent));

    painter.drawPolygon(polygon);

    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;

  rps = std::make_unique<ReponseErasePixelsDiamond>(usersId, *this);
  worker.pushNetwork(std::move(rps));
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
        savetsk = std::make_unique<SaveTask>(itProject->first, std::move(itProject->second.getJson()), std::move(itProject->second.layersImage_));
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
    return std::make_unique<RegisterMessage>(data_packet, c);

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
