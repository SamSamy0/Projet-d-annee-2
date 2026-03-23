#include "LiveProject.hpp"
#include <algorithm>

LiveProject::LiveProject(QJsonObject json) : json_(std::move(json)) {
    
    QJsonArray layers = json_["layers"].toArray();
    for (const QJsonValue &layerValue : layers) {
        if (layerValue.isObject()) {
            QJsonObject layerObj = layerValue.toObject();

            int id = layerObj["id"].toInt();
            layersImage_[id] ;
        }
    }
}

LiveProject::LiveProject(uint id, const QString &projectName, uint width, uint height, uint scale) {
    QJsonObject firstLayer;
    QJsonArray emptylayers;
    
    json_["id"] = static_cast<int>(id);
    json_["name"] = projectName;
    json_["width"] =  static_cast<int>(width);
    json_["height"] =  static_cast<int>(height);
    json_["scale"] = static_cast<int>(scale);
    json_["layerId"] = (int)1;

    firstLayer["type"] = "pixel";
    firstLayer["id"] = 0;
    firstLayer["x"] = 0;
    firstLayer["y"] = 0;

    emptylayers.append(firstLayer);

    json_["layers"] = emptylayers;
    QImage image(width, height , QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    layersImage_[0] = image;
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

void LiveProject::addLayerImage(uint layerId, const QImage& img){
        layersImage_[layerId] = img;
}

uint LiveProject::getScale() {
    return json_["scale"].toInt();
}