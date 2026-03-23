#include "savetask.hpp"

SaveTask::SaveTask(uint projId, QJsonObject json, const std::unordered_map<uint, QImage>& layers) 
    : json_(std::move(json)), projetId_(projId)
{

    for (auto it = layers.begin(); it != layers.end(); ++it) {
        layersImage_.push_back(std::pair(std::move(it->first), it->second.copy()));
    }
}

void SaveTask::execute(ProjectsManager prjManager) {
    prjManager.writeProjetJson(json_, projetId_);
    for (auto layer : layersImage_) {
        prjManager.saveImage(projetId_, layer.first, layer.second);
    }
}