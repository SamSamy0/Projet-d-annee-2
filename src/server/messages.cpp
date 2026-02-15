#include "messages.hpp"
#include "protocol.hpp"



sf::IpAddress Client::getAddress(){
    return sock->getRemoteAddress();
}

 
LoginMessage::LoginMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c): Message(c){
    *data_packet >> pseudo >> password;
}


void LoginMessage::process(Worker& worker){
    //logique de traitement
}



RegisterMessage::RegisterMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c): Message(c){
    *data_packet >> pseudo >> password;
}


void RegisterMessage::process(Worker& worker) {
    //logique de traitement
}


CreateProjectMessage::CreateProjectMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c): Message(c){
    *data_packet >> size >> scale;
}


void CreateProjectMessage::process(Worker& worker) {
    //logique de traitement
}

    
void GetProjectDataMessage::process(Worker& worker) {
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
    };
    
};



