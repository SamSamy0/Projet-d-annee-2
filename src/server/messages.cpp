#include "messages.hpp"
#include "protocol.hpp"
#include "worker.hpp"
#include "reponsequeue.hpp"
#include <utility>

 


LoginMessage::LoginMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c): Message(c){
    *data_packet >> pseudo >> password;
}


void LoginMessage::process(Worker& worker){
    client.id = worker.verifyLogin(this->pseudo, this->password);

    //création de la réponse
    auto rps = std::make_unique<Reponse>();
    rps->client = this->client;
    rps->message_type = MsgProtocole::AUTH_RESULT;

    rps->packet = std::make_unique<sf::Packet>();
    *rps->packet << static_cast<std::uint8_t>(MsgProtocole::AUTH_RESULT);
    if (client.id == -1) {
        *rps->packet << static_cast<std::uint8_t>(0);
    } else {
        *rps->packet << static_cast<std::uint8_t>(1);
    }

    //mise sur la liste des réponses
    worker.rep_queue.push(std::move(rps));
}



RegisterMessage::RegisterMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c): Message(c){
    *data_packet >> pseudo >> password;
}


void RegisterMessage::process(Worker& worker) {
    client.id = worker.addUser(this->pseudo, this->password);
    
    //création de la réponse
    auto rps = std::make_unique<Reponse>();
    rps->client = this->client;
    rps->message_type = MsgProtocole::AUTH_RESULT;

    rps->packet = std::make_unique<sf::Packet>();
    *rps->packet << static_cast<std::uint8_t>(MsgProtocole::AUTH_RESULT);
    if (client.id == -1) {
        *rps->packet << static_cast<std::uint8_t>(0);
    } else {
        *rps->packet << static_cast<std::uint8_t>(1);
    }

    //mise sur la liste des réponses
    worker.rep_queue.push(std::move(rps));

}


CreateProjectMessage::CreateProjectMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c): Message(c){
    *data_packet >> nomProjet >> size.x >> size.y >> scale;
}


void CreateProjectMessage::process(Worker& worker) {
    long long id_proj = worker.db_Manager.addProject("test", this->client.id);
    worker.proj_Manager.createProjectJson(id_proj, "test", size.x, size.y, scale);
}


GetProjectDataMessage::GetProjectDataMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c): Message(c) {
    // à faitre un jour
}


void GetProjectDataMessage::process(Worker& worker) {
    //à faire un jour
}

GetUsersProjectsMessage::GetUsersProjectsMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c): Message(c) {
    *data_packet >> userdId_;

}

void GetUsersProjectsMessage::process(Worker& worker) {
    std::vector<ProjectEntry> projects = worker.db_Manager.getAllProjects();

    auto rps = std::make_unique<Reponse>();
    rps->client = this->client;
    rps->message_type = MsgProtocole::LOB_GET_MY_PROJECTS_DATA_REP;

    rps->packet = std::make_unique<sf::Packet>();
    *rps->packet << static_cast<std::uint8_t>(MsgProtocole::LOB_GET_MY_PROJECTS_DATA_REP);

    *rps->packet << static_cast<std::uint32_t>(projects.size());

    for (const auto& entry : projects) {
        *rps->packet << static_cast<std::uint32_t>(projects.size());
        *rps->packet << entry.name;
        *rps->packet << entry.role;
    }

    worker.rep_queue.push(std::move(rps));
}

std::unique_ptr<Message> MessageFactory(std::shared_ptr<sf::Packet> data_packet,Client client){
    uint8_t message_type;
    *data_packet >> message_type;

    auto type = static_cast<MsgProtocole>(message_type);

    switch (type) {
        case MsgProtocole::AUTH_LOGIN_REQ: 
            return std::make_unique<LoginMessage>(data_packet, client);
        
        case MsgProtocole::AUTH_REGISTER_REQ:
            return std::make_unique<RegisterMessage>(data_packet, client);

        case MsgProtocole::LOB_CREATE_PROJECT_REQ:
            return std::make_unique<CreateProjectMessage>(data_packet, client);
        
        case MsgProtocole::LOB_PROJECT_LIST_REQ:
            return std::make_unique<GetProjectDataMessage>(data_packet, client);
    };
    
};



