#include "LiveProject.hpp"
#include <algorithm>
#include <QPainter>
#include <QImage>

LiveProject::LiveProject(uint projId) {
    ProjectsManager prjManager;
    QJsonObject json_ = prjManager.loadProjectJson(projId);
    QJsonArray layers = json_["layers"].toArray();


    scale_ = json_["scale"].toInt();
    width_ = json_["width"].toInt();
    height_ = json_["height"].toInt();
    id_ = json_["id"].toInt();
    name_ = json_["name"].toString().toStdString();

    layers_ = LayerManager(layers, projId,  json_["nextLayerId"].toInt(), height_, width_, scale_);
}

LiveProject::LiveProject(uint id, const std::string &projectName, uint width, uint height, uint scale) 
    : layers_(height, width, scale), name_(projectName), scale_(scale), height_(height),  width_(width), 
    id_(id) { }

void LiveProject::addConnection(uint userId, uint8_t role) {
    auto it = std::find(connectedID_.begin(), connectedID_.end(), userId);
    if (it == connectedID_.end()) {
        connectedID_.push_back(userId);
        usersRoles_[userId] = role;}
}

bool LiveProject::removeConnection(uint userId) {
    auto it = std::find(connectedID_.begin(), connectedID_.end(), userId);
    if (it != connectedID_.end()) {
        
        *it = connectedID_.back(); 
        connectedID_.pop_back();
    }

    return connectedID_.empty();
}

QJsonObject LiveProject::getJson() {
    QJsonObject json;
    json["height"] = static_cast<int>(height_);
    json["width"] = static_cast<int>(width_);
    json["scale"] = static_cast<int>(scale_);
    json["id"] = static_cast<int>(id_);
    json["name"] = QString::fromStdString(name_);
    json["nextLayerId"] = static_cast<int>(layers_.getNextLayerId());

    json["layers"] = layers_.getJson();
    return json;
}

std::vector<uint>& LiveProject::getConnected() {
    return connectedID_;
}


uint LiveProject::getScale() {
    return scale_;
}

bool LiveProject::drawPixelRect(uint userId, uint calqueId, uint x, uint y, float taille,
    uint8_t r, uint8_t g, uint8_t b, uint8_t op) {

    if (!canModify(userId)) {
        return false;
    }
    
    return layers_.drawPixelRect(calqueId, x, y, taille, r, g, b, op);
}

bool LiveProject::drawPixelCircle(uint userId, uint calqueId, uint x, uint y, float taille,
    uint8_t r, uint8_t g, uint8_t b, uint8_t op) {
    
    if (!canModify(userId)) {
        return false;
    }

    return layers_.drawPixelCircle(calqueId, x, y, taille, r, g, b, op);
}

bool LiveProject::drawPixelDiam(uint userId, uint calqueId, uint x, uint y, float h, float w, 
    uint8_t r, uint8_t g, uint8_t b, uint8_t op) {

    if (!canModify(userId)) {
        return false;
    }

    return layers_.drawPixelDiam(calqueId, x, y, h, w, r, g, b, op);
}

bool LiveProject::erasePixelRect(uint userId, uint calqueId, uint x, uint y, float taille) {
    
    if (!canModify(userId)) {
        return false;
    }
    return layers_.erasePixelRect(calqueId, x, y, taille);
}

bool LiveProject::erasePixelCircle(uint userId, uint calqueId, uint x, uint y, float taille) {
    
    if (!canModify(userId)) {
        return false;
    }

    return layers_.erasePixelCircle(calqueId, x, y, taille);
}

bool LiveProject::erasePixelDiam(uint userId, uint calqueId, uint x, uint y, float h, float w) {

    if (!canModify(userId)) {
        return false;
    }

    return layers_.erasePixelDiam(calqueId, x, y, h, w);
}

const std::unordered_map<uint, SpriteLayer>& LiveProject::getSpritesMap() {
    return layers_.getSpritesMap();
}

const std::unordered_map<uint, QImage>& LiveProject::getImageMap() {
    return layers_.getImageMap();
}

bool LiveProject::addSprite(uint userId, uint calqueId, std::string asset_id, uint x, uint y, float taille) {
    if (!canModify(userId)) {
        return false;
    }

    return layers_.addSprite(calqueId, asset_id, x, y, taille);
}

bool LiveProject::addCalque(uint userId, uint8_t type) {
    if (!canModify(userId)) {
        return false;
    }

    return layers_.addCalque(type);
}

bool LiveProject::removeCalque(uint userId, uint calqueId) {
    if (!canModify(userId)) {
        return false;
    }

    return layers_.removeCalque(calqueId);
}

bool LiveProject::shiftCalque(uint userId, uint calqueId, uint deltaX, uint deltaY) {
    if (!canModify(userId)) {
        return false;
    }

    return layers_.shiftCalque(calqueId, deltaX, deltaY);
}

bool LiveProject::moveCalqueUp(uint userId, uint calqueId) {
    
    if (!canModify(userId)) {
        return false;
    }

   return layers_.moveCalqueUp(calqueId); 
}

bool LiveProject::moveCalqueDown(uint userId, uint calqueId) {
       
    if (!canModify(userId)) {
        return false;
    }

   return layers_.moveCalqueDown(calqueId); 
}

bool LiveProject::canModify(uint userId) {
    auto cleVal = usersRoles_.find(userId);
    if (cleVal == usersRoles_.end()) {
        return false;
    }

    if (cleVal->second == 0 ){
        return false;
    }
    return true;
}

bool LiveProject::removeSprite(uint userId, uint calqueId, uint spriteId) {
    
    if (!canModify(userId)) {
        return false;
    }

    return layers_.removeSprite(calqueId, spriteId);
}
