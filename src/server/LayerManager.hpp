#pragma once
#include <unordered_map>
#include <list>
#include <QJsonArray>
#include <QImage>
#include "SpriteLayer.hpp"

struct Layer {
    uint8_t type; //0=pixel 1=sprite
    uint id;
    int x;
    int y;
    std::string name;
};

class LayerManager {
    uint scale_;
    uint height_;
    uint width_;

    std::list<Layer> layers_;
    uint lastLayerId_;
    std::unordered_map<uint, std::list<Layer>::iterator> mapId_;

    std::unordered_map<uint, QImage> layersImage_;
    std::unordered_map<uint, SpriteLayer> layersSprite_;

    void setupLayerSprite(uint LayerId, const QJsonObject& sprites);

    public:
    LayerManager(const QJsonArray& origin, uint projectId, uint lastUsedId, uint height, uint width, uint scale);
    LayerManager(uint height, uint width, uint scale);
    LayerManager() = default;
    bool drawPixelRect(uint calqueId, uint x, uint y, float taille, uint8_t r, uint8_t g, uint8_t b, uint8_t op);
    bool drawPixelCircle(uint calqueId, uint x, uint y, float taille, uint8_t r, uint8_t g, uint8_t b, uint8_t op);
    bool drawPixelDiam(uint calqueId, uint x, uint y, float h, float w, uint8_t r, uint8_t g, uint8_t b, uint8_t op);
    bool erasePixelRect(uint calqueId, uint x, uint y, float taille);
    bool erasePixelCircle(uint calqueId, uint x, uint y, float taille);
    bool erasePixelDiam(uint calqueId, uint x, uint y, float h, float w);
    
    bool addCalquePixel();
    bool addCalqueSprite();
    bool addCalqueSpriteAfter(uint calqueId);
    bool addCalque( uint8_t type);
    bool removeCalque(uint calqueId);
    
    bool addSprite(uint calqueId, std::string asset_id, uint x, uint y, float taille);
    bool removeSprite(uint calqueId, uint spriteId);
    bool moveSprite(uint calqueId, std::vector<uint> spriteIds, int deltaX, int deltaY);
    bool resizeSprite(uint calqueId, std::vector<uint> spriteIds, float scale, std::vector<float> x, std::vector<float> y);
    bool rotateSprite(uint calqueId, std::vector<uint> spriteIds, float angle, std::vector<float> x, std::vector<float> y);
    bool autoFill(uint calqueId, const std::vector<std::string>& asset_ids, float angle, float size, const std::vector<sf::Vector2f>& positions);

    bool moveCalqueUp(uint calqueId);
    bool moveCalqueDown(uint calqueId);
    bool shiftCalque(uint calqueId, uint deltaX, uint deltaY);
    bool renameCalque(uint calqueId, std::string newName);

    struct LayerInfo {
    uint id;
    uint8_t type;
    int x, y;
};
    std::vector<LayerManager::LayerInfo> getLayersInfo()const;

    const std::unordered_map<uint, SpriteLayer>& getSpritesMap();
    const std::unordered_map<uint, QImage>& getImageMap();

    const QJsonArray getJson();
    uint getNextLayerId();

    const std::vector<uint> getLayerOrder();


};
