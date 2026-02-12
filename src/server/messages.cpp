#include <SFML/Network.hpp>
#include <vector>
#include <iostream>
#include <array>
#include <memory>
#include <deque>
#include "protocol.hpp"


struct Client{
    int id = 0;
    std::shared_ptr<sf::TcpSocket> sock;
    sf::IpAddress getAddress(){
        return sock->getRemoteAddress();
    }
};

 
struct Message {
    Client client;

    explicit Message(const Client& c) : client(c) {}
    virtual void process() = 0;
    virtual ~Message() = default;
};


struct LoginMessage : Message{
    std::string pseudo;
    std::string password;
    LoginMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c)
        : Message(c)
    {
        *data_packet >> pseudo >> password;
    }
    void process() override{
        //logique de traitement
    }
};


struct RegisterMessage : Message{
    std::string pseudo;
    std::string password;
    RegisterMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c)
        : Message(c)
    {
        *data_packet >> pseudo >> password;
    }
    void process() override{
        //logique de traitement
    }
};


struct CreateProjectMessage : Message{
    float size;
    float scale;
    CreateProjectMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c)
        : Message(c)
    {
        *data_packet >> size >> scale;
    }
    void process() override{
        //logique de traitement
    }
};


struct GetProjectDataMessage : Message{
    
    void process() override{
        //logique de traitement
    }
};


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



