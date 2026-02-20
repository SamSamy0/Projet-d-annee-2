#ifndef CLIENTNETWORK_HPP
#define CLIENTNETWORK_HPP

#include <memory>
#include <deque>
#include <SFML/Network.hpp>
#include "protocol.hpp"



struct ServerEvent{
    MsgProtocole message_type_;
    std::unique_ptr<sf::Packet> data_packet_;
};


class ClientNetworkManager{
    sf::TcpSocket socket_;
    std::deque<ServerEvent> reponse_;
    
public:
        
    
    bool connect();
    void getEvent();
    bool hasEvent();
    ServerEvent popEvent();
    std::deque<ServerEvent>& getQueuEvent();
    void login(std::string pseudo,std::string password);
    void askRegister(std::string pseudo, std::string password);
    void createProject(std::string role, sf::Vector2u size, float scale);
    void getProjectData(int project_id);
    void getUsersProjects(long long id);

        
};

#endif



