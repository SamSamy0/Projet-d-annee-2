#include "savetask.hpp"

SaveTask::SaveTask(LiveProject& livePrj, uint projId) 
    : json_(livePrj.getJson()), projetId_(projId)
{
    std::unordered_map<uint, QImage>& layersImagePrj = livePrj.getImageMap();
    for (auto it = layersImagePrj.begin(); it != layersImagePrj.end(); ++it) {
        layersImage_.emplace_back(it->first, it->second);
    }

    std::unordered_map<uint, std::vector<Sprite>>& layersSpritePrj = livePrj.getSpritesMap();
    for (auto it = layersSpritePrj.begin(); it != layersSpritePrj.end(); ++it) {
        layersSprite_.emplace_back(it->first, it->second);
    }
}

void SaveTask::execute(ProjectsManager prjManager) {
    prjManager.writeProjetJson(json_, projetId_);
    for (auto layer : layersImage_) {
        prjManager.saveImage(projetId_, layer.first, layer.second);
    }

    for(auto layer : layersSprite_) {
        QJsonArray layerJson;
        for (auto srpiteOpt : layer.second) {
            QJsonObject objetSprite;
            objetSprite["interId"] = static_cast<int>(srpiteOpt.id);
            objetSprite["nameId"] = QString::fromStdString(srpiteOpt.nameId);
            objetSprite["x"] = static_cast<int>(srpiteOpt.x);
            objetSprite["y"] = static_cast<int>(srpiteOpt.y);
            objetSprite["size"] = static_cast<float>(srpiteOpt.taille);
            layerJson.append(objetSprite);
        }
        prjManager.saveSprite(projetId_, layer.first, layerJson);
    }
}