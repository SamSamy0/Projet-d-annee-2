#pragma once
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <deque>
#include <unordered_map>
#include <QImage>
#include "datamanager/projectsmanager.hpp"
#include "SpriteLayer.hpp"

struct LiveProject {
    LiveProject(uint projId);
    LiveProject(uint id, const QString &projectName, uint width, uint height, uint scale);
    void addConnection(uint userId, uint8_t role);
    bool removeConnection(uint userId); //return true if the project is empty
    std::vector<uint>& getConnected();
    QJsonObject& getJson();
    bool drawPixelRect(uint userId, uint calqueId, uint x, uint y, float taille, uint8_t r, uint8_t g, uint8_t b, uint8_t op);
    bool drawPixelCircle(uint userId, uint calqueId, uint x, uint y, float taille, uint8_t r, uint8_t g, uint8_t b, uint8_t op);
    bool drawPixelDiam(uint userId, uint calqueId, uint x, uint y, float h, float w, uint8_t r, uint8_t g, uint8_t b, uint8_t op);
    bool erasePixelRect(uint userId, uint calqueId, uint x, uint y, float taille);
    bool erasePixelCircle(uint userId, uint calqueId, uint x, uint y, float taille);
    bool erasePixelDiam(uint userId, uint calqueId, uint x, uint y, float h, float w);
    bool addCalquePixel(uint userId);
    bool addCalqueSprite(uint userId);
    bool addSprite(uint userId, uint calqueId_, std::string asset_id, uint x, uint y, float taille);


    uint getScale();
    std::unordered_map<uint, SpriteLayer>& getSpritesMap();
    std::unordered_map<uint, QImage>& getImageMap();

    private :
    QJsonObject json_;
    QJsonArray layers_;
    std::vector<uint> connectedID_;
    std::unordered_map<uint, int8_t> usersRoles_;
    std::unordered_map<uint, QImage> layersImage_;
    std::unordered_map<uint, SpriteLayer> layersSprite_;
    uint scale_;
    uint lastUsedLayerId_;
    uint height_;
    uint width_;
    void setupLayerSprite(uint layerId, const QJsonObject& sprite);

};