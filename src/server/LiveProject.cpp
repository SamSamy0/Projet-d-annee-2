#include "LiveProject.hpp"

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