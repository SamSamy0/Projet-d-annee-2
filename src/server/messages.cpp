#include "messages.hpp"
#include "protocol.hpp"
<<<<<<< HEAD
#include "worker.hpp"
#include "reponsequeue.hpp"
=======



>>>>>>> 130851d (avancement handler)

 
sf::IpAddress Client::getAddress(){
    return sock->getRemoteAddress();
}

LoginMessage::LoginMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c): Message(c){
    *data_packet >> pseudo >> password;
}


void LoginMessage::process(Worker& worker){
    client.id = worker.verifyLogin(this->pseudo, this->password);
    Reponse rps;
    rps.client = this->client;
    //rps.packet = 
}



RegisterMessage::RegisterMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c): Message(c){
    *data_packet >> pseudo >> password;
}


void RegisterMessage::process(Worker& worker) {
    client.id = worker.verifyLogin(this->pseudo, this->password);
    Reponse rps;

}


CreateProjectMessage::CreateProjectMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c): Message(c){
    *data_packet >> size >> scale;
}


void CreateProjectMessage::process(Worker& worker) {
    //logique de traitement
}

<<<<<<< HEAD
    
void GetProjectDataMessage::process(Worker& worker) {
=======

    //logique de traitement
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



