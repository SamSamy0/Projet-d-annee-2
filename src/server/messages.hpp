#pragma once
#include <memory>
#include "client.hpp"
#include <SFML/Network.hpp>


class Worker;

//cette structure est utilisé pour représenter les messages recu par les clients
struct Message {
    std::shared_ptr<Client> client_; // On pointe vers le client au lieu de le copier

    explicit Message(std::shared_ptr<Client> c) : client_(c) {}
    virtual void process(Worker& worker) = 0;
    virtual ~Message() = default;
};

//message de connexion
struct LoginMessage : Message{
    std::string pseudo_;
    std::string password_;
    
    LoginMessage(std::shared_ptr<sf::Packet> data_packet, std::shared_ptr<Client> c);
    void process(Worker& worker) override;

};

//messages d'inscription
struct RegisterMessage : Message{
    std::string pseudo_;
    std::string password_;
    
    RegisterMessage(std::shared_ptr<sf::Packet> data_packet, std::shared_ptr<Client> c);
    void process(Worker& worker) override;
};

//message de creation de projet
struct CreateProjectMessage : Message{
    std::string nomProjet_;
    sf::Vector2u size_;
    uint scale_;
    
    CreateProjectMessage(std::shared_ptr<sf::Packet> data_packet, std::shared_ptr<Client> c);
    void process(Worker& worker) override;
};

struct GetProjectDataMessage : Message{
    int projectId_;

    GetProjectDataMessage(std::shared_ptr<sf::Packet> data_packet, std::shared_ptr<Client> c);
    void process(Worker& worker) override;
};

struct GetUsersProjectsMessage : Message {
    int userdId_;

    GetUsersProjectsMessage(std::shared_ptr<sf::Packet> data_packet, std::shared_ptr<Client> c);
    void process(Worker& worker) override;
};


std::shared_ptr<Message> MessageFactory(std::shared_ptr<sf::Packet> data_packet, std::shared_ptr<Client> c);




