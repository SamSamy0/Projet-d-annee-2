#ifndef CLIENTNETWORK_HPP
#define CLIENTNETWORK_HPP

#include <memory>
#include <deque>
#include <SFML/Network.hpp>
#include "protocol.hpp"


struct ServerEvent{
    MsgProtocole message_type;
    std::unique_ptr<sf::Packet> data_packet;
};


class ClientNetworkManager{
    sf::TcpSocket socket;
    std::deque<ServerEvent> reponse;
    
public:
        
    std::deque<ServerEvent>& getQueuEvent();
    bool connect();
    void getEvent();
    bool hasEvent();
    ServerEvent popEvent();
    void login(std::string pseudo,std::string password);
    void askRegister(std::string pseudo, std::string password);
    void createProject(std::string role, sf::Vector2u size, float scale);
    
    //
    void getProjectData(int project_id);

        
};

#endif



