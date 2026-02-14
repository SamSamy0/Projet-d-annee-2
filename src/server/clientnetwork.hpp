#ifndef CLIENTNETWORK_HPP
#define CLIENTNETWORK_HPP

#include <memory>
#include <deque>
#include <SFML/Network.hpp>
#include "protocol.hpp"


struct ServerRep{
    MsgProtocole message_type;
    std::unique_ptr<sf::Packet> data_packet;
};


class ClientNetworkManager{
    sf::TcpSocket socket;
    std::deque<ServerRep> reponse;
    
public:
        
    std::deque<ServerRep>& getQueuRep();
    bool connect();
    void getReponse();
    void login(std::string pseudo,std::string password);
    void askRegister(std::string pseudo, std::string password);
    void createProject(std::string role, float size, float scale);

        
};

#endif



