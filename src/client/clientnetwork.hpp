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
    void renameProject(int projectId, std::string newName);
    void dupProj(int projectId, std::string newName);
    void getProjectData(int project_id);
    void getUsersProjects(long long id);
    void getProjectList(); 
    void delProject(long long project_id);
    void getProjectData(long long project_id);
    void createProjectCode(long long project_id);
    void joinProject(int project_code);
    
    void drawSquare(int pos_x,int pos_y, float size, uint8_t r, uint8_t g,uint8_t b, uint8_t a);
    void drawCircle(int pos_x,int pos_y,float size, uint8_t r, uint8_t g,uint8_t b, uint8_t a);
    void drawDiamond(int pos_x,int pos_y,float size_x, float size, uint8_t r, uint8_t g,uint8_t b, uint8_t a);
    void eraseSquare(int pos_x,int pos_y,float size);
    void eraseCircle(int pos_x,int pos_y,float size);
    void eraseDiamaond(int pos_x,int pos_y,float size_x,float size_y);
    
    void sendMessageChat();

};





