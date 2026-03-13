#pragma once
#include <memory>
#include <deque>
#include <SFML/Network.hpp>
#include "protocol.hpp"
#include "serverevent.hpp"


class ClientEventHandler;


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
    void createProject(std::string role, sf::Vector2u size, uint scale);
    void renameProject(int projectId, std::string newName);
    void dupProj(int projectId, std::string newName);
    void getProjectData(int project_id);
    void getUsersProjects(long long id);
    void getProjectList();     
};





