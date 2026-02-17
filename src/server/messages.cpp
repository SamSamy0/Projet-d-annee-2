#include "messages.hpp"
#include "protocol.hpp"
#include "worker.hpp"
#include "reponsequeue.hpp"
#include <utility>

 
sf::IpAddress Client::getAddress(){
    return sock->getRemoteAddress();
}

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
    *rps->packet << static_cast<sf::Uint8>(MsgProtocole::AUTH_RESULT);
    if (client.id == -1) {
        *rps->packet << static_cast<sf::Uint8>(0);
    } else {
        *rps->packet << static_cast<sf::Uint8>(1);
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
    *rps->packet << static_cast<sf::Uint8>(MsgProtocole::AUTH_RESULT);
    if (client.id == -1) {
        *rps->packet << static_cast<sf::Uint8>(0);
    } else {
        *rps->packet << static_cast<sf::Uint8>(1);
    }

    //mise sur la liste des réponses
    worker.rep_queue.push(std::move(rps));

}


CreateProjectMessage::CreateProjectMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c): Message(c){
    *data_packet >> size >> scale;
}


void CreateProjectMessage::process(Worker& worker) {
    long long id_proj = worker.db_Manager.addProject("test", this->client.id);
    worker.proj_Manager.createProjectJson(id_proj, "test", size, scale);
}
    
    int projectId_;

void GetProjectDataMessage::process(Worker& worker) {
    
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
        
        case MsgProtocole::LOB_PROJECT_DATA_REQ:
            return std::make_unique<GetProjectDataMessage>(data_packet, client);
    };
    
};



