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
    void getUsersProjects(uint64_t id);
    void getProjectList(); 
    void delProject(uint64_t project_id);
    void getProjectData(uint64_t project_id);
    void createProjectCode(uint64_t project_id);
    void joinProject(int project_code);
    
    void drawSquare(uint64_t proj_id, uint64_t layer_id, int pos_x,int pos_y, float size, uint8_t r, uint8_t g,uint8_t b, uint8_t a);
    void drawCircle(uint64_t proj_id, uint64_t layer_id,int pos_x,int pos_y,float size, uint8_t r, uint8_t g,uint8_t b, uint8_t a);
    void drawDiamond(uint64_t proj_id, uint64_t layer_id,int pos_x,int pos_y,float size_x, uint8_t r, uint8_t g,uint8_t b, uint8_t a);
    void eraseSquare(uint64_t proj_id, uint64_t layer_id,int pos_x,int pos_y,float size);
    void eraseCircle(uint64_t proj_id, uint64_t layer_id,int pos_x,int pos_y,float size);
    void eraseDiamond(uint64_t proj_id, uint64_t layer_id,int pos_x,int pos_y,float size_x,float size_y);
    
    void sendMessageChat();

};





