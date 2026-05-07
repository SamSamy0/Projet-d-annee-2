#pragma once
#include "sprite.hpp"
#include <QJsonObject>
#include <SFML/System/Vector2.hpp>


class SpriteLayer {
    std::list<Sprite> sprites_;
    uint lastId_;
    std::unordered_map<uint, std::list<Sprite>::iterator> mapId_;

    public :
    SpriteLayer(const QJsonObject& origin);
    SpriteLayer();
    QJsonObject load() const ;
    void addSprite(std::string nameId, float taille, uint x, uint y, float angle = 0.0);
    bool eraseSprite(uint idSprite);
    void moveSprite(std::vector<uint> spriteIds, int deltaX, int deltaY);
    void resizeSprite(std::vector<uint> spriteIds, float scale, std::vector<float> x, std::vector<float> y);
    void rotateSprite(std::vector<uint> spriteIds, float angle, std::vector<float> x, std::vector<float> y);
    void autoFill(const std::vector<std::string>& asset_ids, float angle, float size, const std::vector<sf::Vector2f>& positions);
};