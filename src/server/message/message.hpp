#pragma once
#include <iostream>
#include <SFML/Network.hpp>
#include "../client.hpp"

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

struct RenameProjectMessage: IMessage{
    long long userID_;
    int projectId_;
    std::string newName_;

    RenameProjectMessage(sf::Packet& dataPacket, long long userId);
    void process(Worker& worker) override;
    
};

struct DuplicateProjectMessage: IMessage{
    long long userId_;
    int projectId_;
    std::string newName;
    
    DuplicateProjectMessage(sf::Packet& dataPacket, long long userId);
    void process(Worker& worker) override;
};


struct GetProjectsListMessage : IMessage {
    long long userId_;

    GetProjectsListMessage(sf::Packet& dataPacket, long long userId);
    void process(Worker& worker) override;
};


struct DeleteProjectMessage : IMessage {
    long long userId_;
    int projectId_;

    DeleteProjectMessage(sf::Packet& dataPacket, long long userId);
    void process(Worker& worker) override;
};


struct GetProjectDataMessage : IMessage{
    long long userId_;
    int projectId_;

    GetProjectDataMessage(sf::Packet& dataPacket, long long userId);
    void process(Worker& worker) override;
};

struct PutPixelsMessage : IMessage {
    long long userId_;
    int projectId_;
    int calqueId_;
    sf::Vector2u pos_;
    uint8_t red_;
    uint8_t green_;
    uint8_t blue_;
    uint8_t opa_;
};

struct PutPixelsCircleMessage : PutPixelsMessage {

    float taille_;
    
    PutPixelsCircleMessage(sf::Packet& dataPacket, long long userId);
    void process(Worker& worker) override;
};

struct PutPixelsCarreMessage : PutPixelsMessage {

    float taille_;

    PutPixelsCarreMessage(sf::Packet& dataPacket, long long userId);
    void process(Worker& worker) override;
};

struct PutPixelsLosangeMessage : PutPixelsMessage {

    float hauteur_;
    float largeur_;

    PutPixelsLosangeMessage(sf::Packet& dataPacket, long long userId);
    void process(Worker& worker) override;
};

struct ErasePixelsMessage : IMessage {
    long long userId_;
    int projectId_;
    int calqueId_;
    sf::Vector2u pos_;
};

struct ErasePixelsCircleMessage : ErasePixelsMessage {

    float taille_;
    
    ErasePixelsCircleMessage(sf::Packet& dataPacket, long long userId);
    void process(Worker& worker) override;
};

struct ErasePixelsCarreMessage : ErasePixelsMessage {

    float taille_;

    ErasePixelsCarreMessage(sf::Packet& dataPacket, long long userId);
    void process(Worker& worker) override;
};

struct ErasePixelsLosangeMessage : ErasePixelsMessage {

    float hauteur_;
    float largeur_;

    ErasePixelsLosangeMessage(sf::Packet& dataPacket, long long userId);
    void process(Worker& worker) override;
};


std::unique_ptr<IMessage> MessageFactory(sf::Packet& data_packet, std::shared_ptr<Client> client);
