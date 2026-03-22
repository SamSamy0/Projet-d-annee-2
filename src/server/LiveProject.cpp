#include "LiveProject.hpp"
#include <algorithm>

LiveProject::LiveProject(QJsonObject json) : json_(std::move(json)) {
    
    QJsonArray layers = json_["layers"].toArray();
    for (const QJsonValue &layerValue : layers) {
        if (layerValue.isObject()) {
            QJsonObject layerObj = layerValue.toObject();

            int id = layerObj["id"].toInt();
            mapModif_[id];
        }
    }
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

void LiveProject::addModif(ModifProject& modifPrj) {

}

QJsonObject& LiveProject::getJson() {
    return json_;
}

std::vector<uint>& LiveProject::getConnected() {
    return connectedID_;
}