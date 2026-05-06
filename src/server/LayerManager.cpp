#include "LayerManager.hpp"
#include "datamanager/projectsmanager.hpp"
#include <QImage>
#include <QPainter>
#include <utility>
#include <iterator>

LayerManager::LayerManager(const QJsonArray& origin, uint projectId, uint lastUsedId, uint height, uint width, uint scale) {
    ProjectsManager prjManager;
    lastLayerId_ = lastUsedId;
    height_ = height;
    width_ = width;
    scale_ = scale;

    for (const QJsonValue &layerValue : origin) {
        if (layerValue.isObject()) {
            QJsonObject layerObj = layerValue.toObject();
            Layer layer;
            int id = layerObj["id"].toInt();
            layer.id = id;
            layer.x = layerObj["x"].toInt();
            layer.y = layerObj["y"].toInt();
            layer.name = layerObj["name"].toString().toStdString();

            if (layerObj["type"].toInt() == 0){
                layersImage_[id] = prjManager.loadImage(projectId, id);
                layer.type = 0;
            }
            else {
                layersSprite_[id];
                setupLayerSprite(id, prjManager.loadSpriteLayer(projectId, id));
                layer.type = 1;
            }
            layers_.push_back(std::move(layer));
            auto it = std::prev(layers_.end());
            mapId_[it->id] = it;
        }
    }

    qDebug()<<scale_ << " " << height_ << " " << width_;
}

void LayerManager::setupLayerSprite(uint LayerId, const QJsonObject& sprites){
    if (layersSprite_.find(LayerId) != layersSprite_.end()) {
        layersSprite_[LayerId] = SpriteLayer(sprites);
    }
}
std::vector<LayerManager::LayerInfo> LayerManager::getLayersInfo() const {
    std::vector<LayerInfo> result;
    for (const auto& layer : layers_) {
        result.push_back({layer.id, layer.type, layer.x, layer.y});
    }
    return result;
}

LayerManager::LayerManager(uint height, uint width, uint scale) {
    height_ = height;
    width_ = width;
    scale_ = scale;

    Layer layer{0,0,0,0, "Couche Pixel (1)"};
    layers_.push_back(std::move(layer));
    auto it = std::prev(layers_.end());
    mapId_[it->id] = it;
    lastLayerId_ = 1;

    QImage image(width_, height_, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    layersImage_[0] = image;
}

bool LayerManager::drawPixelRect(uint calqueId, uint x, uint y, float taille, uint8_t r, uint8_t g, uint8_t b, uint8_t op) {
    
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

    return false;
}

bool LayerManager::drawPixelCircle(uint calqueId, uint x, uint y, float taille, uint8_t r, uint8_t g, uint8_t b, uint8_t op) {
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
    
    return false;
}
    
bool LayerManager::drawPixelDiam(uint calqueId, uint x, uint y, float h, float w, uint8_t r, uint8_t g, uint8_t b, uint8_t op) {
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
    return false;
}

bool LayerManager::erasePixelRect(uint calqueId, uint x, uint y, float taille) {
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

    return false;
}
    
bool LayerManager::erasePixelCircle(uint calqueId, uint x, uint y, float taille) {
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
    return false;
}
    
bool LayerManager::erasePixelDiam(uint calqueId, uint x, uint y, float h, float w) {
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
    
    return false;
}
    
const std::unordered_map<uint, SpriteLayer>& LayerManager::getSpritesMap() {
    return layersSprite_;
}

const std::unordered_map<uint, QImage>& LayerManager::getImageMap() {
    return layersImage_;
}

bool LayerManager::addCalquePixel() {
    Layer newLayer{0, lastLayerId_, 0, 0, "Couche Pixel (" + std::to_string(lastLayerId_ + 1) + ")"};

    layers_.push_back(newLayer);
    auto it = std::prev(layers_.end());
    mapId_[it->id] = it;


    QImage image(width_, height_ , QImage::Format_ARGB32);
    qDebug() << width_ << " " << height_ ;
    image.fill(Qt::transparent);
    layersImage_[lastLayerId_] = image;

    lastLayerId_ += 1;
    return true;
}

bool LayerManager::addCalqueSprite() {
    Layer newLayer{1, lastLayerId_, 0, 0, "Couche Sprite (" + std::to_string(lastLayerId_ + 1) + ")"};

    layers_.push_back(newLayer);
    auto it = std::prev(layers_.end());
    mapId_[it->id] = it; 

    layersSprite_[lastLayerId_];

    lastLayerId_ += 1;
    return true;
}
    
bool LayerManager::addCalque( uint8_t type) {

    if (type == 0) {
        return addCalquePixel();
    } else {
        return addCalqueSprite();
    }
}
    
bool LayerManager::removeCalque(uint calqueId) {
    auto it = mapId_.find(calqueId);
    if (it != mapId_.end()) {
        uint8_t type = it->second->type;
        layers_.erase(it->second);
        mapId_.erase(it);
        if (type == 0) {
            layersImage_.erase(calqueId);
        } else {
            layersSprite_.erase(calqueId);
        }
        return true;
    }
    return false;
}

bool LayerManager::addSprite(uint calqueId, std::string asset_id, uint x, uint y, float taille) {
    if (layersSprite_.find(calqueId) != layersSprite_.end()) {
        layersSprite_[calqueId].addSprite(asset_id, taille, x, y);
        return true;        
    }
    return false;
}

bool LayerManager::removeSprite(uint calqueId, uint spriteId) {
    if (layersSprite_.find(calqueId) != layersSprite_.end()) {
        layersSprite_[calqueId].eraseSprite(spriteId);
        return true;        
    }
    return false;
}

bool LayerManager::moveSprite(uint calqueId, std::vector<uint> spriteIds, int deltaX, int deltaY) {
    if (layersSprite_.find(calqueId) != layersSprite_.end()) {
        layersSprite_[calqueId].moveSprite(spriteIds, deltaX, deltaY);
        qDebug() << "MoveSprite: calqueId=" << calqueId << " deltaX=" << deltaX << " deltaY=" << deltaY;
        return true;        
    }
    return false;
}

bool LayerManager::resizeSprite(uint calqueId, std::vector<uint> spriteIds, float scale, std::vector<float> x, std::vector<float> y) {
    if (layersSprite_.find(calqueId) != layersSprite_.end()) {
        layersSprite_[calqueId].resizeSprite(spriteIds, scale, x, y);
        return true;        
    }
    return false;
}

bool LayerManager::rotateSprite(uint calqueId, std::vector<uint> spriteIds, float angle, std::vector<float> x, std::vector<float> y) {
    if (layersSprite_.find(calqueId) != layersSprite_.end()) {
        layersSprite_[calqueId].rotateSprite(spriteIds, angle, x, y);
        return true;        
    }
    return false;
}
    
bool LayerManager::moveCalqueUp(uint calqueId) {
    auto it = mapId_.find(calqueId);
    
    if (it != mapId_.end()) {
        auto list_it = it->second; 
        if (list_it != layers_.begin()) {
            auto it_prev = std::prev(list_it);
            layers_.splice(it_prev, layers_, list_it);
            return true; 
        }
    } 
    return false;
}

bool LayerManager::moveCalqueDown(uint calqueId) {
    auto it = mapId_.find(calqueId);
    
    if (it != mapId_.end()) {
        auto list_it = it->second; 
        auto next_it = std::next(list_it);
        if (next_it != layers_.end()) {
            layers_.splice(list_it, layers_, next_it);
            return true;
        }
    }
    return false;
}

const QJsonArray LayerManager::getJson() {
    QJsonArray array;
    for (auto& layer : layers_) {
        QJsonObject object;
        object["id"] = static_cast<int>(layer.id);
        object["x"] = static_cast<int>(layer.x);
        object["y"] = static_cast<int>(layer.y);
        object["type"] = static_cast<int>(layer.type);
        object["name"] = QString::fromStdString(layer.name);
        array.append(object);
    }

    return array;
}

bool LayerManager::shiftCalque(uint calqueId, uint deltaX, uint deltaY) {
    auto it = mapId_.find(calqueId);
    
    if (it != mapId_.end()) {
        it->second->x += deltaX;
        it->second->y += deltaY;
        return true;
    }
    return false;
}

uint LayerManager::getNextLayerId() {
    return lastLayerId_;
}

const std::vector<uint> LayerManager::getLayerOrder() {
    std::vector<uint> order;
    for (const auto& layer : layers_) {
        order.push_back(layer.id);
    }
    return order;
}

bool LayerManager::renameCalque(uint calqueId, std::string newName) {
    auto it = mapId_.find(calqueId);
    
    if (it != mapId_.end()) {
        it->second->name = newName;
        return true;
    }
    return false;
}
