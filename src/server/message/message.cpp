#include "message.hpp"
#include "../worker.hpp"
#include "../../common/protocol.hpp"
#include "../reponse/reponse.hpp"



LoginMessage::LoginMessage(sf::Packet& dataPacket, std::shared_ptr<Client> client) {
    dataPacket >> pseudo_ >> password_;
    client_= client;
}


void LoginMessage::process(Worker& worker){
    long long id = worker.verifyLogin(this->pseudo_, this->password_);

    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseAuth>(std::move(client_), id);
    worker.pushNetwork(std::move(rps));
}



RegisterMessage::RegisterMessage(sf::Packet& dataPacket, std::shared_ptr<Client> client) {
    dataPacket >> pseudo_ >> password_;
    client_ = client;
}


void RegisterMessage::process(Worker& worker) {
    long long id = worker.addUser(this->pseudo_, this->password_);

    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseAuth>(std::move(client_), id);
    worker.pushNetwork(std::move(rps));
}


CreateProjectMessage::CreateProjectMessage(sf::Packet& data_packet, long long userId) {
    data_packet >> nomProjet_ >> size_.x >> size_.y >> scale_;
    userId_ = userId;
}


void CreateProjectMessage::process(Worker& worker) {
    if (userId_ > 0) {
    long long idProj = worker.addProjectSQL(nomProjet_, userId_);
    worker.createProjectJson(idProj, nomProjet_, size_.x, size_.y, scale_);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseCreateProject>(userId_, idProj);
    worker.pushNetwork(std::move(rps));
    }
}

RenameProjectMessage::RenameProjectMessage(sf::Packet& dataPacket, long long userId){
    dataPacket >> projectId_ >> newName_;
    userID_ = userId;
}

void RenameProjectMessage::process(Worker& worker){
    bool success = worker.renameProject(projectId_, newName_);
    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseRenameProject>(userID_, projectId_, newName_, success);
    worker.pushNetwork(std::move(rps));
}

DuplicateProjectMessage::DuplicateProjectMessage(sf::Packet& dataPacket, long long userId){
    dataPacket >> projectId_ >> newName;
    userId_ = userId;
}

void DuplicateProjectMessage::process(Worker& worker){
    bool success = worker.duplicateProject(projectId_, newName, userId_);
    std::unique_ptr<Reponse>rps;
    rps = std::make_unique<ReponseDuplicateProject>(userId_, projectId_, newName, success);
    worker.pushNetwork(std::move(rps));
}


GetProjectsListMessage::GetProjectsListMessage(sf::Packet& data_packet, long long userId) {
    userId_ = userId;
}

void GetProjectsListMessage::process(Worker& worker) {
    std::vector<ProjectEntry> projects;
    if (userId_ != 0) {
        projects = worker.getUserProjects(userId_);
    }
    std::cout << static_cast<int>(projects.size())<<std::endl;

    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseUsersProjects>(userId_, projects);
    worker.pushNetwork(std::move(rps));
}

DeleteProjectMessage::DeleteProjectMessage(sf::Packet& data_packet, long long userId) {
    userId_ = userId;
    data_packet >> projectId_;
}

void DeleteProjectMessage::process(Worker& worker) {
    if (worker.getRole(userId_, projectId_) == 2) {
        worker.deleteProject(projectId_);
    }
}


GetProjectDataMessage::GetProjectDataMessage(sf::Packet& data_packet, long long userId) {
    userId_ = userId;
    data_packet >> projectId_;
}

void GetProjectDataMessage::process(Worker& worker) {
}


std::unique_ptr<IMessage> MessageFactory(sf::Packet& data_packet, std::shared_ptr<Client> c) {
    uint8_t typeRaw;
    if (!(data_packet >> typeRaw)) return nullptr;

    std::cout << static_cast<int>(typeRaw) << "le type du mess" << std::endl;

    MsgProtocole type = static_cast<MsgProtocole>(typeRaw);
    switch (type) {
        case MsgProtocole::AUTH_LOGIN_REQ:
            return std::make_unique<LoginMessage>(data_packet, std::move(c));

        case MsgProtocole::AUTH_REGISTER_REQ:
            return std::make_unique<RegisterMessage>(data_packet, std::move(c));

        case MsgProtocole::LOB_CREATE_PROJECT_REQ:
            std::cout << "écrire un projet à priori" << std::endl;
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
        
        default:
            std::cout<< "pas de message" << std::endl;
            return nullptr;
    }
}
