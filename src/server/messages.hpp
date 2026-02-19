#ifndef MESSAGES_HPP
#define MESSAGES_HPP

#include <memory>
#include "client.hpp"
#include <SFML/Network.hpp>

class Worker;


struct Message {
    Client client;

    explicit Message(const Client& c) : client(c) {}
    virtual void process(Worker& worker) = 0;
    virtual ~Message() = default;
};


struct LoginMessage : Message{
    std::string pseudo;
    std::string password;
    
    LoginMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c);
    void process(Worker& worker) override;

};


struct RegisterMessage : Message{
    std::string pseudo;
    std::string password;
    
    RegisterMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c);
    void process(Worker& worker) override;
};


struct CreateProjectMessage : Message{
    std::string nomProjet;
    sf::Vector2u size;
    float scale;
    
    CreateProjectMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c);
    void process(Worker& worker) override;
};


struct GetProjectDataMessage : Message{
    int projectId_;

    GetProjectDataMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c);
    void process(Worker& worker) override;
};

struct GetUsersProjectsMessage : Message {
    int userdId_;

    GetUsersProjectsMessage(std::shared_ptr<sf::Packet> data_packet, const Client& c);
    void process(Worker& worker) override;
};


std::unique_ptr<Message> MessageFactory(std::shared_ptr<sf::Packet> data_packet,Client client);




#endif