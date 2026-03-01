#pragma once
#include <iostream>
#include <SFML/Network.hpp>
#include "client.hpp"

class Worker;

class IMessage {
    public:
    virtual void process(Worker& worker) = 0;
    virtual ~IMessage() = default;
};



struct LoginMessage : public IMessage{
    std::shared_ptr<Client> client_;
    std::string pseudo_;
    std::string password_;
    
    LoginMessage(sf::Packet& dataPacket, std::shared_ptr<Client> client);
    void process(Worker& worker) override;

};


struct RegisterMessage : IMessage{
    std::shared_ptr<Client> client_;
    std::string pseudo_;
    std::string password_;
    
    RegisterMessage(sf::Packet& dataPacket, std::shared_ptr<Client> client);
    void process(Worker& worker) override;
};


struct CreateProjectMessage : IMessage{
    long long userId_;
    std::string nomProjet_;
    sf::Vector2u size_;
    uint scale_;
    
    CreateProjectMessage(sf::Packet& dataPacket, long long userId);
    void process(Worker& worker) override;
};


struct GetProjectDataMessage : IMessage{
    long long userId_;
    int projectId_;

    GetProjectDataMessage(sf::Packet& dataPacket, long long userId);
    void process(Worker& worker) override;
};

struct GetUsersProjectsMessage : IMessage {
    long long userId_;

    GetUsersProjectsMessage(sf::Packet& dataPacket, long long userId);
    void process(Worker& worker) override;
};


std::unique_ptr<IMessage> MessageFactory(sf::Packet& data_packet, std::shared_ptr<Client> client);
