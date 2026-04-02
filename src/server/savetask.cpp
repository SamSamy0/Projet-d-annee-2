#include "savetask.hpp"

SaveTask::SaveTask(LiveProject& livePrj, uint projId) 
    : json_(livePrj.getJson()), projetId_(projId)
{
    std::unordered_map<uint, QImage>& layersImagePrj = livePrj.getImageMap();
    for (auto it = layersImagePrj.begin(); it != layersImagePrj.end(); ++it) {
        layersImage_.emplace_back(it->first, it->second);
    }

    std::unordered_map<uint, SpriteLayer>& layersSpritePrj = livePrj.getSpritesMap();
    for (auto it = layersSpritePrj.begin(); it != layersSpritePrj.end(); ++it) {
        layersSprite_.emplace_back(it->first, it->second.load());
    }
}

void SaveTask::execute(ProjectsManager prjManager) {
    prjManager.writeProjetJson(json_, projetId_);
    for (auto layer : layersImage_) {
        prjManager.saveImage(projetId_, layer.first, layer.second);
    }

    for(auto layer : layersSprite_) {
        prjManager.saveSprite(projetId_, layer.first, layer.second);
    }
}