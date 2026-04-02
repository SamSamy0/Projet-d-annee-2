#pragma once
#include "sprite.hpp"
#include <QJsonObject>


class SpriteLayer {
    std::list<Sprite> sprites_;
    uint lastId_;
    std::unordered_map<uint, std::list<Sprite>::iterator> mapId_;

    public :
    SpriteLayer(const QJsonObject& origin);
    SpriteLayer();
    QJsonObject load();
    void addSprite(std::string nameId, float taille, uint x, uint y);
    bool eraseSprite(uint idSprite);
};