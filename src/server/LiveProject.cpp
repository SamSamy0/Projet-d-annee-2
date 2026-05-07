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
    chat_ = Chat(prjManager.loadChat(projId));
    spriteManager_ = SpriteManager(prjManager.loadSprites(projId));
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
        return true;
    }

    return false;
}

bool LiveProject::isEmpty() {
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

    if (!spriteManager_.verifyAssetId(asset_id)) {
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

bool LiveProject::moveSprite(uint userId, uint calqueId, std::vector<uint> spriteIds, int deltaX, int deltaY) {
    if (!canModify(userId)) {
        qDebug() << "User not allowed to modify the project";
        return false;
    }

    return layers_.moveSprite(calqueId, spriteIds, deltaX, deltaY);
}

bool LiveProject::resizeSprite(uint userId, uint calqueId, std::vector<uint> spriteIds, float scale, std::vector<float> x, std::vector<float> y) {
    if (!canModify(userId)) {
        return false;
    }

    return layers_.resizeSprite(calqueId, spriteIds, scale, x, y);
}

bool LiveProject::rotateSprite(uint userId, uint calqueId, std::vector<uint> spriteIds, float angle, std::vector<float> x, std::vector<float> y) {
    if (!canModify(userId)) {
        return false;
    }

    return layers_.rotateSprite(calqueId, spriteIds, angle, x, y);
}

const std::vector<uint> LiveProject::getLayerOrder() {
    return layers_.getLayerOrder();
}

bool LiveProject::renameCalque(uint userId, uint calqueId, std::string newName) {
    if (!canModify(userId)) {
        return false;
    }

    return layers_.renameCalque(calqueId, newName);
}

const QJsonArray LiveProject::getChatJson() {
    return chat_.toJson();
}

bool LiveProject::addMessageChat(uint userId, const std::shared_ptr<MessageChat> message) {
    auto cleVal = usersRoles_.find(userId);
    if (cleVal == usersRoles_.end()) {
        qDebug() << "User not found in project";
        return false;
    }
    
    chat_.addMessage(message);
    return true;
}

uint LiveProject::importSprite(uint userId, sf::Texture &sprite) {
    if (!canModify(userId)) {
        qDebug() << "User not allowed to modify the project";
        return -1;
    }

    return spriteManager_.addSprite(sprite);
}

const std::map<uint, sf::Texture>& LiveProject::getSpriteManagerMap() {
    return spriteManager_.getImportedSprites();
}

bool LiveProject::autoFill(uint userId, uint calqueId, const std::vector<std::string>& asset_ids, float angle, float size_, const std::vector<sf::Vector2f>& positions_) {
    if (!canModify(userId)) {
        qDebug() << "User not allowed to modify the project";
        return false;
    }

    return layers_.autoFill(calqueId, asset_ids, angle, size_, positions_);
}