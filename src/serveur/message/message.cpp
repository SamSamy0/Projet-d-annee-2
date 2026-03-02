#include "message.hpp"
#include "../worker.hpp"
#include "protocol.hpp"
#include "../reponse/reponse.hpp"



LoginMessage::LoginMessage(sf::Packet& dataPacket, std::shared_ptr<Client> client) {
    dataPacket >> pseudo_ >> password_;
    client_= client;
}


void LoginMessage::process(Worker& worker){
    long long id = worker.verifyLogin(this->pseudo_, this->password_);

    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseAuth>(client_, id);
    worker.pushNetwork(std::move(rps));
}



RegisterMessage::RegisterMessage(sf::Packet& dataPacket, std::shared_ptr<Client> client) {
    dataPacket >> pseudo_ >> password_;
    client_ = client;
}


void RegisterMessage::process(Worker& worker) {
    long long id = worker.addUser(this->pseudo_, this->password_);

    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseAuth>(client_, id);
    worker.pushNetwork(std::move(rps));
}


CreateProjectMessage::CreateProjectMessage(sf::Packet& data_packet, long long userId) {
    data_packet >> nomProjet_ >> size_.x >> size_.y >> scale_;
    userId_ = userId;
}


void CreateProjectMessage::process(Worker& worker) {
    if (userId_ >= 0) {
    long long idProj = worker.addProjectSQL(nomProjet_, userId_);
    worker.createProjectJson(idProj, nomProjet_, size_.x, size_.y, scale_);
    }
}


GetProjectDataMessage::GetProjectDataMessage(sf::Packet& data_packet, long long userId) {
}


void GetProjectDataMessage::process(Worker& worker) {
}

GetUsersProjectsMessage::GetUsersProjectsMessage(sf::Packet& data_packet, long long userId) {
    userId_ = userId;
}

void GetUsersProjectsMessage::process(Worker& worker) {
    std::vector<ProjectEntry> projects;
    if (userId_ == 0) {
        projects = worker.getAllProjects();
    }
    std::cout << static_cast<int>(projects.size())<<std::endl;

    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseUsersProjects>(userId_, projects);
    worker.pushNetwork(std::move(rps));
}


std::unique_ptr<IMessage> MessageFactory(sf::Packet& data_packet, std::shared_ptr<Client> c) {
    uint8_t typeRaw;
    if (!(data_packet >> typeRaw)) return nullptr;

    MsgProtocole type = static_cast<MsgProtocole>(typeRaw);

    switch (type) {
        case MsgProtocole::AUTH_LOGIN_REQ:
            return std::make_unique<LoginMessage>(data_packet, c);

        case MsgProtocole::AUTH_REGISTER_REQ:
            return std::make_unique<RegisterMessage>(data_packet, c);

        case MsgProtocole::LOB_CREATE_PROJECT_REQ:
            return std::make_unique<CreateProjectMessage>(data_packet, c->id);

        case MsgProtocole::LOB_GET_MY_PROJECTS_DATA_REQ:
            return std::make_unique<GetUsersProjectsMessage>(data_packet, c->id);
        default:
            return nullptr;
    }
}