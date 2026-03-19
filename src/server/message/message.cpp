#include "message.hpp"
#include "../worker.hpp"
#include "../../common/protocol.hpp"
#include "../reponse/reponse.hpp"



LoginMessage::LoginMessage(sf::Packet& dataPacket, std::shared_ptr<Client> client) {
    dataPacket >> pseudo_ >> password_;
    client_= client;
}


void LoginMessage::process(Worker& worker){
    uint id = worker.verifyLogin(this->pseudo_, this->password_);

    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseAuth>(std::move(client_), id);
    worker.pushNetwork(std::move(rps));
}



RegisterMessage::RegisterMessage(sf::Packet& dataPacket, std::shared_ptr<Client> client) {
    dataPacket >> pseudo_ >> password_;
    client_ = client;
}


void RegisterMessage::process(Worker& worker) {
    uint id = worker.addUser(this->pseudo_, this->password_);

    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseAuth>(std::move(client_), id);
    worker.pushNetwork(std::move(rps));
}


CreateProjectMessage::CreateProjectMessage(sf::Packet& data_packet, uint userId) {
    data_packet >> nomProjet_ >> size_.x >> size_.y >> scale_;
    userId_ = userId;
}


void CreateProjectMessage::process(Worker& worker) {
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

RenameProjectMessage::RenameProjectMessage(sf::Packet& dataPacket, uint userId){
    dataPacket >> projectId_ >> newName_;
    userID_ = userId;
}

void RenameProjectMessage::process(Worker& worker){
    bool success = worker.renameProject(projectId_, newName_);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseRenameProject>(userID_, projectId_, newName_, success);
    worker.pushNetwork(std::move(rps));
}

DuplicateProjectMessage::DuplicateProjectMessage(sf::Packet& dataPacket, uint userId){
    dataPacket >> projectId_ >> newName;
    userId_ = userId;
}

void DuplicateProjectMessage::process(Worker& worker){
    long long newId = worker.duplicateProject(projectId_, newName, userId_);
    std::unique_ptr<Reponse>rps;
    if (newId != -1){
        rps = std::make_unique<ReponseDuplicateProject>(userId_, newId, newName);
        worker.pushNetwork(std::move(rps));
    }
}


GetProjectsListMessage::GetProjectsListMessage(sf::Packet& data_packet, uint userId) {
    userId_ = userId;
}

void GetProjectsListMessage::process(Worker& worker) {
    std::vector<ProjectEntry> projects;
    if (userId_ != 0) {
        projects = worker.getUserProjects(userId_);
    }

    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseUsersProjects>(userId_, projects);
    worker.pushNetwork(std::move(rps));
}

DeleteProjectMessage::DeleteProjectMessage(sf::Packet& data_packet, uint userId) {
    userId_ = userId;
    data_packet >> projectId_;
}

void DeleteProjectMessage::process(Worker& worker) {
    if (worker.getRole(userId_, projectId_) == 2) {
        worker.deleteProject(projectId_);
    }
}


GetProjectDataMessage::GetProjectDataMessage(sf::Packet& data_packet, uint userId) {
    userId_ = userId;
    data_packet >> projectId_;
}

void GetProjectDataMessage::process(Worker& worker) {

    if (worker.mapProjet_.find(projectId_) == worker.mapProjet_.end()) {
        auto liveProj = LiveProject(worker.loadProjectJson(projectId_));
        liveProj.connectedID_.push_back(userId_);
        liveProj.userRole_[userId_] = worker.getRole(userId_, projectId_);
        worker.mapProjet_.emplace(projectId_, std::move(liveProj));

    }
    else {
        auto item_id_Projet = (worker.mapProjet_).find(projectId_);
        worker.writeProjetJson(item_id_Projet->second.json_, projectId_);
        item_id_Projet->second.connectedID_.push_back(userId_);
        item_id_Projet->second.userRole_[userId_] = worker.getRole(userId_, projectId_);

    }
    QByteArray jsonData = worker.getByteJson(projectId_);

    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseProjectData>(userId_, jsonData);
    worker.pushNetwork(std::move(rps));
}


std::vector<uint> ModifProjetMessage::getUserLists(Worker& worker) {
    std::vector<uint> usersId = worker.mapProjet_.find(projectId_)->second.connectedID_;

    auto it = std::find(usersId.begin(), usersId.end(), userId_);
    if (it != usersId.end()) {
        
        *it = usersId.back(); 
        usersId.pop_back();
    }
    return usersId;
}

PutPixelsCarreMessage::PutPixelsCarreMessage(sf::Packet& data_packet, uint userId) {
    userId_ = userId;
    data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> red_ >> green_ >> blue_ >> opa_ >> taille_;
}

void PutPixelsCarreMessage::process(Worker& worker) {

    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;

    rps = std::make_unique<ReponsePutPixelsCarre>(usersId, *this);
    worker.pushNetwork(std::move(rps));
}

PutPixelsCircleMessage::PutPixelsCircleMessage(sf::Packet& data_packet, uint userId) {
    userId_ = userId;
    data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> red_ >> green_ >> blue_ >> opa_ >> taille_;
}

void PutPixelsCircleMessage::process(Worker& worker) {

    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;

    rps = std::make_unique<ReponsePutPixelsCircle>(usersId, *this);
    worker.pushNetwork(std::move(rps));
}

PutPixelsDiamondMessage::PutPixelsDiamondMessage(sf::Packet& data_packet, uint userId) {
    userId_ = userId;
    data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> red_ >> green_ >> blue_ >> opa_ >> hauteur_ >> largeur_;
}

void PutPixelsDiamondMessage::process(Worker& worker) {

    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;

    rps = std::make_unique<ReponsePutPixelsDiamond>(usersId, *this);
    worker.pushNetwork(std::move(rps));
}

ErasePixelsCarreMessage::ErasePixelsCarreMessage(sf::Packet& data_packet, uint userId) {
    userId_ = userId;
    data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_;
}

void ErasePixelsCarreMessage::process(Worker& worker) {

    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;

    rps = std::make_unique<ReponseErasePixelsCarre>(usersId, *this);
    worker.pushNetwork(std::move(rps));
}

ErasePixelsCircleMessage::ErasePixelsCircleMessage(sf::Packet& data_packet, uint userId) {
    userId_ = userId;
    data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> taille_;
}

void ErasePixelsCircleMessage::process(Worker& worker) {

    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;

    rps = std::make_unique<ReponseErasePixelsCircle>(usersId, *this);
    worker.pushNetwork(std::move(rps));
}

ErasePixelsDiamondMessage::ErasePixelsDiamondMessage(sf::Packet& data_packet, uint userId) {
    userId_ = userId;
    data_packet >> projectId_ >> calqueId_ >> pos_.x >> pos_.y >> hauteur_ >> largeur_;
}

void ErasePixelsDiamondMessage::process(Worker& worker) {

    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;

    rps = std::make_unique<ReponseErasePixelsDiamond>(usersId, *this);
    worker.pushNetwork(std::move(rps));
}

MoveLayerMessage::MoveLayerMessage(sf::Packet& data_packet, uint userId) {
    userId_ = userId;
    data_packet >> projectId_ >> calqueId_ >>  deltaX_ >> deltaY_;
}

void MoveLayerMessage::process(Worker& worker) {

    std::vector<uint> usersId = this->getUserLists(worker);
    std::unique_ptr<Reponse> rps;

    rps = std::make_unique<ReponseMoveLayer>(usersId, *this);
    worker.pushNetwork(std::move(rps));
}


std::unique_ptr<IMessage> MessageFactory(sf::Packet& data_packet, std::shared_ptr<Client> c) {
    uint8_t typeRaw;
    if (!(data_packet >> typeRaw)) return nullptr;

    MsgProtocole type = static_cast<MsgProtocole>(typeRaw);
    std::cout << "[From client " << c->id <<"]:"<< to_string(type) <<std::endl;
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
            return std::make_unique<GetProjectDataMessage>(data_packet,c->id);
        
        case MsgProtocole::LOB_RENAME_PROJECT_REQ:
            return std::make_unique<RenameProjectMessage>(data_packet, c->id);
        
        case MsgProtocole::LOB_DUPLICATE_PROJECT_REQ:
            return std::make_unique<DuplicateProjectMessage>(data_packet, c->id);

        case MsgProtocole::MAP_PUT_PIXELS_CARRE_REQ:
            return std::make_unique<PutPixelsCarreMessage>(data_packet, c->id);

        case MsgProtocole::MAP_PUT_PIXELS_CIRCLE_REQ:
            return std::make_unique<PutPixelsCircleMessage>(data_packet, c->id);

        case MsgProtocole::MAP_PUT_PIXELS_DIAM_REQ:
            return std::make_unique<PutPixelsDiamondMessage>(data_packet, c->id);

        case MsgProtocole::MAP_ERASE_PIXELS_CARRE_REQ:
            return std::make_unique<ErasePixelsCarreMessage>(data_packet, c->id);

        case MsgProtocole::MAP_ERASE_PIXELS_CIRCLE_REQ:
            return std::make_unique<ErasePixelsCircleMessage>(data_packet, c->id);

        case MsgProtocole::MAP_ERASE_PIXELS_DIAM_REQ:
            return std::make_unique<ErasePixelsDiamondMessage>(data_packet, c->id);

        case MsgProtocole::MAP_MOV_LAYER_REQ:
            return std::make_unique<MoveLayerMessage>(data_packet, c->id);

        default:
            std::cout<< "pas de message" << std::endl;
            return nullptr;
    }
}
