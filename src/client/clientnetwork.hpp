#pragma once
#include <memory>
#include <deque>
#include <SFML/Network.hpp>
#include "../common/protocol.hpp"
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
    void renameProject(uint projectId, std::string newName);
    void dupProj(uint projectId, std::string newName);
    void getProjectData(uint project_id);
    void getUsersProjects(uint id);
    void getProjectList(); 
    void delProject(uint project_id);
    void getProjectData(uint project_id);
    void createProjectCode(uint project_id);
    void joinProject(uint project_code);
    
    void syncPaint();
    void sendMessageChat();

};





