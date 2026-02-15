#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <memory>
#include <SFML/Network.hpp>



struct Client{
    int id = 0;
    std::shared_ptr<sf::TcpSocket> sock;
    
    sf::IpAddress getAddress();
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
    
    LoginMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c);
    void process() override;

};


struct RegisterMessage : Message{
    std::string pseudo;
    std::string password;
    
    RegisterMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c);
    void process() override;
};


struct CreateProjectMessage : Message{
    float size;
    float scale;
    
    CreateProjectMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c);
    void process() override;
};


struct GetProjectDataMessage : Message{
    
    void process() override;
};


std::unique_ptr<Message> MessageFactory(std::shared_ptr<sf::Packet> data_packet,Client client);




#endif