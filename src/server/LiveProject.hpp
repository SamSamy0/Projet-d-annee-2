#pragma once
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <deque>
#include <unordered_map>
#include <QImage>
#include "datamanager/projectsmanager.hpp"
#include "SpriteLayer.hpp"
#include "LayerManager.hpp"

struct LiveProject {
    LiveProject(uint projId);
    LiveProject(uint id, const std::string &projectName, uint width, uint height, uint scale);
    void addConnection(uint userId, uint8_t role);
    bool removeConnection(uint userId); //return true if the project is empty
    std::vector<uint>& getConnected();
    QJsonObject getJson();
    bool drawPixelRect(uint userId, uint calqueId, uint x, uint y, float taille, uint8_t r, uint8_t g, uint8_t b, uint8_t op);
    bool drawPixelCircle(uint userId, uint calqueId, uint x, uint y, float taille, uint8_t r, uint8_t g, uint8_t b, uint8_t op);
    bool drawPixelDiam(uint userId, uint calqueId, uint x, uint y, float h, float w, uint8_t r, uint8_t g, uint8_t b, uint8_t op);
    
    bool erasePixelRect(uint userId, uint calqueId, uint x, uint y, float taille);
    bool erasePixelCircle(uint userId, uint calqueId, uint x, uint y, float taille);
    bool erasePixelDiam(uint userId, uint calqueId, uint x, uint y, float h, float w);

    bool addCalque(uint userId, uint8_t type);
    bool removeCalque(uint userId, uint calqueId);
    bool shiftCalque(uint userId, uint calqueId, uint deltaX, uint deltaY);

    bool addSprite(uint userId, uint calqueId, std::string asset_id, uint x, uint y, float taille);
    bool removeSprite(uint userId, uint calqueId, uint spriteId);
    
    bool moveCalqueUp(uint userId, uint calqueId);
    bool moveCalqueDown(uint userId, uint calqueId);


    uint getScale();
    const std::unordered_map<uint, SpriteLayer>& getSpritesMap();
    const std::unordered_map<uint, QImage>& getImageMap();

    private :
    std::vector<uint> connectedID_;
    std::unordered_map<uint, int8_t> usersRoles_;
    uint id_;
    std::string name_;
    uint scale_;
    uint height_;
    uint width_;
    LayerManager layers_;

    
    bool canModify(uint userId);

};
