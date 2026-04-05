#include "SpriteLayer.hpp"
#include <QJsonArray>

SpriteLayer::SpriteLayer(const QJsonObject& origin) {
    lastId_ = origin["lastId"].toInt();
    QJsonArray sprites = origin["sprites"].toArray();
    for (const QJsonValue &sprite : sprites) {
        QJsonObject objetSprite = sprite.toObject();
        Sprite spriteOpt;

        spriteOpt.id = objetSprite["interId"].toInt();
        spriteOpt.nameId = objetSprite["nameId"].toString().toStdString();
        spriteOpt.x = objetSprite["x"].toInt();
        spriteOpt.y = objetSprite["y"].toInt();
        spriteOpt.taille = objetSprite["size"].toDouble();

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
    layer["lastId"] = static_cast<int>(lastId_);
    QJsonArray sprites;
    for (const Sprite &sprite : sprites_) {
        QJsonObject objetSprite;
        objetSprite["interId"] = static_cast<int>(sprite.id);
        objetSprite["nameId"] = QString::fromStdString(sprite.nameId);
        objetSprite["x"] = static_cast<int>(sprite.x);
        objetSprite["y"] = static_cast<int>(sprite.y);
        objetSprite["size"] = static_cast<float>(sprite.taille);
        sprites.append(objetSprite);
    }
    layer["sprites"] = sprites;

    return layer;
}