#include "SpriteLayer.hpp"
#include <QJsonArray>

SpriteLayer::SpriteLayer(const QJsonObject& origin) {
    lastId_ = origin["nextId"].toInt();
    QJsonArray sprites = origin["sprites"].toArray();
    for (const QJsonValue &sprite : sprites) {
        QJsonObject objetSprite = sprite.toObject();
        Sprite spriteOpt;

        spriteOpt.id = objetSprite["interId"].toInt();
        spriteOpt.nameId = objetSprite["nameId"].toString().toStdString();
        spriteOpt.x = objetSprite["x"].toInt();
        spriteOpt.y = objetSprite["y"].toInt();
        spriteOpt.taille = objetSprite["size"].toDouble();
        spriteOpt.angle = objetSprite["angle"].toDouble();
        sprites_.push_back(std::move(spriteOpt));

        auto it = std::prev(sprites_.end());
        mapId_[it->id] = it;
    }
}

SpriteLayer::SpriteLayer() {
    lastId_ = 0;
}

void SpriteLayer::addSprite(std::string nameId, float taille, uint x, uint y) {
    Sprite spriteOpt;

    spriteOpt.id = lastId_;
    lastId_ += 1;

    spriteOpt.nameId = nameId;
    spriteOpt.x = x;
    spriteOpt.y = y;
    spriteOpt.taille = taille;
    spriteOpt.angle = 0.0;
    sprites_.push_back(std::move(spriteOpt));
    
    auto it = std::prev(sprites_.end());
    mapId_[it->id] = it;
}

bool SpriteLayer::eraseSprite(uint idSprite) {
    auto it = mapId_.find(idSprite);
    if (it != mapId_.end()) {
        sprites_.erase(it->second);
        mapId_.erase(it);
        return true;
    }
    return false;
}

QJsonObject SpriteLayer::load() const {
    QJsonObject layer;
    layer["nextId"] = static_cast<int>(lastId_);
    QJsonArray sprites;
    for (const Sprite &sprite : sprites_) {
        QJsonObject objetSprite;
        objetSprite["interId"] = static_cast<int>(sprite.id);
        objetSprite["nameId"] = QString::fromStdString(sprite.nameId);
        objetSprite["x"] = static_cast<int>(sprite.x);
        objetSprite["y"] = static_cast<int>(sprite.y);
        objetSprite["size"] = static_cast<float>(sprite.taille);
        objetSprite["angle"] = static_cast<float>(sprite.angle);
        sprites.append(objetSprite);
    }
    layer["sprites"] = sprites;

    return layer;
}

void SpriteLayer::moveSprite(std::vector<uint> spriteIds, int deltaX, int deltaY) {
    for (uint id : spriteIds) {
        qDebug() << "Moving sprite with ID: " << id << " by (" << deltaX << ", " << deltaY << ")";
        auto it = mapId_.find(id);
        if (it != mapId_.end()) {
            it->second->x += deltaX;
            it->second->y += deltaY;
        }
    }
}

void SpriteLayer::resizeSprite(std::vector<uint> spriteIds, float scale, std::vector<float> x, std::vector<float> y) {
    for (size_t i = 0; i < spriteIds.size(); ++i) {
        uint id = spriteIds[i];
        auto it = mapId_.find(id);
        if (it != mapId_.end()) {
            it->second->taille *= scale;
            it->second->x = x[i];
            it->second->y = y[i];
        }
    }
}

void SpriteLayer::rotateSprite(std::vector<uint> spriteIds, float angle, std::vector<float> x, std::vector<float> y) {
    for (size_t i = 0; i < spriteIds.size(); ++i) {
        uint id = spriteIds[i];
        auto it = mapId_.find(id);
        if (it != mapId_.end()) {
            it->second->angle += angle;
            it->second->x = x[i];
            it->second->y = y[i];
        }
    }
}