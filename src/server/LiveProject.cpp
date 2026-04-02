#include "LiveProject.hpp"
#include <algorithm>
#include <QPainter>
#include <QImage>

LiveProject::LiveProject(uint projId) {
    ProjectsManager prjManager;
    json_ = prjManager.loadProjectJson(projId);
    layers_ = json_["layers"].toArray();
    for (const QJsonValue &layerValue : layers_) {
        if (layerValue.isObject()) {
            QJsonObject layerObj = layerValue.toObject();

            int id = layerObj["id"].toInt();
            if (layerObj["type"].toString().toStdString() == "pixel"){
                layersImage_[id] = prjManager.loadImage(projId, id);
            }
            else {
                layersSprite_[id];
                setupLayerSprite(id, prjManager.loadSprite(projId, id));
            }
        }
    }
    scale_ = json_["scale"].toInt();
    lastUsedLayerId_ = json_["layerId"].toInt();
    width_ = json_["width"].toInt();
    height_ = json_["height"].toInt();

}

LiveProject::LiveProject(uint id, const QString &projectName, uint width, uint height, uint scale) {
    QJsonObject firstLayer;
    QJsonObject secondLayer;
    
    json_["id"] = static_cast<int>(id);
    json_["name"] = projectName;
    json_["width"] =  static_cast<int>(width);
    json_["height"] =  static_cast<int>(height);
    json_["scale"] = static_cast<int>(scale);
    json_["layerId"] = (int)2;

    firstLayer["type"] = "pixel";
    firstLayer["id"] = 0;
    firstLayer["x"] = 0;
    firstLayer["y"] = 0;

    secondLayer["type"] = "sprite"; //
    secondLayer["id"] = 1;
    secondLayer["x"] = 0;
    secondLayer["y"] = 0;

    scale_ = scale;
    lastUsedLayerId_ = 2;//
    height_ = height;
    width_ = width;

    layers_.append(firstLayer);
    layers_.append(secondLayer);//

    json_["layers"] = layers_;
    QImage image(width, height , QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    layersImage_[0] = image;

    layersSprite_[1] = SpriteLayer();
    qDebug() << "nv liveporj";
}

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

QJsonObject& LiveProject::getJson() {
    return json_;
}

std::vector<uint>& LiveProject::getConnected() {
    return connectedID_;
}

void LiveProject::setupLayerSprite(uint LayerId, const QJsonObject& sprites){
    if (layersSprite_.find(LayerId) != layersSprite_.end()) {
        layersSprite_[LayerId] = SpriteLayer(sprites);
    }
}

uint LiveProject::getScale() {
    return scale_;
}

bool LiveProject::drawPixelRect(uint userId, uint calqueId, uint x, uint y, float taille,
    uint8_t r, uint8_t g, uint8_t b, uint8_t op) {
    
    auto cleVal = usersRoles_.find(userId);
    if (cleVal == usersRoles_.end()) {
        return false;
    }

    if (cleVal->second != 0 ){
        if (layersImage_.find(calqueId) != layersImage_.end()) {
        QPainter painter(&layersImage_[calqueId]);
        QColor color(r, g, b, op);
        
        painter.setRenderHint(QPainter::Antialiasing, false);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(color));
        float topLeftX = x - (taille*scale_)/2.0f;
        float topLeftY = y - (taille*scale_)/2.0f;
        
        painter.drawRect(QRectF(topLeftX, topLeftY, taille * scale_, taille * scale_));
        return true;
        }
    }
    return false;
}

bool LiveProject::drawPixelCircle(uint userId, uint calqueId, uint x, uint y, float taille,
    uint8_t r, uint8_t g, uint8_t b, uint8_t op) {
    
    auto cleVal = usersRoles_.find(userId);
    if (cleVal == usersRoles_.end()) {
        return false;
    }

    if (cleVal->second != 0 ){
        if (layersImage_.find(calqueId) != layersImage_.end()) {
            QPainter painter(&layersImage_[calqueId]);
            QColor color(r, g, b, op);
            QPolygonF polygon;

            for (int i = 0; i < 30; ++i) {

                float angle = i * 2 * M_PI / 30;
                float px = x + std::cos(angle) * taille * scale_/ 2.0f;
                float py = y + std::sin(angle) * taille * scale_ / 2.0f;
                polygon << QPointF(px, py);
        }

        painter.setRenderHint(QPainter::Antialiasing, false);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(color));

        painter.drawPolygon(polygon);
        return true;
        }
    }
    return false;
}

bool LiveProject::drawPixelDiam(uint userId, uint calqueId, uint x, uint y, float h, float w, 
    uint8_t r, uint8_t g, uint8_t b, uint8_t op) {

    auto cleVal = usersRoles_.find(userId);
    if (cleVal == usersRoles_.end()) {
        return false;
    }

    if (cleVal->second != 0 ){
        if (layersImage_.find(calqueId) != layersImage_.end()) {
            QPainter painter(&layersImage_[calqueId]);
            QColor color(r, g, b, op);
            QPolygonF polygon;

            float h_demi = h * scale_/ 2.0f;
            float l_demi = w * scale_ / 2.0f;
            polygon << QPointF(x, y - h_demi);
            polygon << QPointF(x + l_demi, y);
            polygon << QPointF(x, y + h_demi);
            polygon << QPointF(x - l_demi, y);

            painter.setRenderHint(QPainter::Antialiasing, false);
            painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(color));

            painter.drawPolygon(polygon);
            return true;
        }
    }
    return false;
}

bool LiveProject::erasePixelRect(uint userId, uint calqueId, uint x, uint y, float taille) {
    
    auto cleVal = usersRoles_.find(userId);
    if (cleVal == usersRoles_.end()) {
        return false;
    }

    if (cleVal->second != 0 ){
        if (layersImage_.find(calqueId) != layersImage_.end()) {
            QPainter painter(&layersImage_[calqueId]);
        
            painter.setRenderHint(QPainter::Antialiasing, false);
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(Qt::transparent));
            float topLeftX = x - (taille*scale_)/2.0f;
            float topLeftY = y - (taille*scale_)/2.0f;
        
            painter.drawRect(QRectF(topLeftX, topLeftY, taille * scale_, taille * scale_));
            return true;
        }
    }
    return false;
}

bool LiveProject::erasePixelCircle(uint userId, uint calqueId, uint x, uint y, float taille) {
    
    auto cleVal = usersRoles_.find(userId);
    if (cleVal == usersRoles_.end()) {
        return false;
    }

    if (cleVal->second != 0 ){
        if (layersImage_.find(calqueId) != layersImage_.end()) {
            QPainter painter(&layersImage_[calqueId]);
            QPolygonF polygon;

            for (int i = 0; i < 30; ++i) {

                float angle = i * 2 * M_PI / 30;
                float px = x + std::cos(angle) * taille * scale_/ 2.0f;
                float py = y + std::sin(angle) * taille * scale_ / 2.0f;
                polygon << QPointF(px, py);
            }

            painter.setRenderHint(QPainter::Antialiasing, false);
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(Qt::transparent));

            painter.drawPolygon(polygon);
            return true;
        }
    }
    return false;
}

bool LiveProject::erasePixelDiam(uint userId, uint calqueId, uint x, uint y, float h, float w) {

    auto cleVal = usersRoles_.find(userId);
    if (cleVal == usersRoles_.end()) {
        return false;
    }

    if (cleVal->second != 0 ){
        if (layersImage_.find(calqueId) != layersImage_.end()) {
            QPainter painter(&layersImage_[calqueId]);
            QPolygonF polygon;

            float h_demi = h * scale_/ 2.0f;
            float l_demi = w * scale_ / 2.0f;
            polygon << QPointF(x, y - h_demi);
            polygon << QPointF(x + l_demi, y);
            polygon << QPointF(x, y + h_demi);
            polygon << QPointF(x - l_demi, y);

            painter.setRenderHint(QPainter::Antialiasing, false);
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QBrush(Qt::transparent));

            painter.drawPolygon(polygon);
            return true;
        }
    }
    return false;
}

std::unordered_map<uint, SpriteLayer>& LiveProject::getSpritesMap() {
    return layersSprite_;
}

std::unordered_map<uint, QImage>& LiveProject::getImageMap() {
    return layersImage_;
}

bool LiveProject::addCalquePixel(uint userId) {

    auto cleVal = usersRoles_.find(userId);
    if (cleVal == usersRoles_.end()) {
        return false;
    }

    if (cleVal->second != 0 ) {

        QJsonObject newLayer;
        newLayer["type"] = "pixel";
        newLayer["id"] = static_cast<int>(lastUsedLayerId_);
        newLayer["x"] = 0;
        newLayer["y"] = 0;        

        layers_.append(newLayer);
        QImage image(width_, height_ , QImage::Format_ARGB32);
        image.fill(Qt::transparent);
        layersImage_[lastUsedLayerId_] = image;

        lastUsedLayerId_ += 1;
        json_["layerId"] = static_cast<int>(lastUsedLayerId_);
        return true;
    }

    return false;
}

bool LiveProject::addCalqueSprite(uint userId) {

    auto cleVal = usersRoles_.find(userId);
    if (cleVal == usersRoles_.end()) {
        return false;
    }

    if (cleVal->second != 0 ) {

        QJsonObject newLayer;
        newLayer["type"] = "sprite";
        newLayer["id"] = static_cast<int>(lastUsedLayerId_);
        newLayer["x"] = 0;
        newLayer["y"] = 0;        

        layers_.append(newLayer);
        layersSprite_[lastUsedLayerId_];

        lastUsedLayerId_ += 1;
        json_["layerId"] = static_cast<int>(lastUsedLayerId_);
        return true;
    }
    
    return false;
}

bool LiveProject::addSprite(uint userId, uint calqueId, std::string asset_id, uint x, uint y, float taille) {
    auto cleVal = usersRoles_.find(userId);
    if (cleVal == usersRoles_.end()) {
        return false;
    }

    if (cleVal->second = 0 ){
        return false;
    }

    if (layersSprite_.find(calqueId) != layersSprite_.end()) {
        layersSprite_[calqueId].addSprite(asset_id, taille, x, y);
        return true;        
    }
    return false;
}