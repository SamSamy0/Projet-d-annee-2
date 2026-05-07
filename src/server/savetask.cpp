#include "savetask.hpp"
#include "reponse/reponse.hpp"
#include "saveworker.hpp"

SaveTask::SaveTask(LiveProject& livePrj, uint projId) 
    : json_(livePrj.getJson()), projetId_(projId), chat_(livePrj.getChatJson())
    , sprites_(livePrj.getSpriteManagerMap())
{
    const std::unordered_map<uint, QImage>& layersImagePrj = livePrj.getImageMap();
    for (auto it = layersImagePrj.begin(); it != layersImagePrj.end(); ++it) {
        layersImage_.emplace_back(it->first, it->second);
    }

  const std::unordered_map<uint, SpriteLayer> &layersSpritePrj =
      livePrj.getSpritesMap();
  for (auto it = layersSpritePrj.begin(); it != layersSpritePrj.end(); ++it) {
    layersSprite_.emplace_back(it->first, it->second.load());
  }
}

void SaveTask::execute(SaveWorker &worker) {
  ProjectsManager prjManager = worker.getPrjMngr();
  prjManager.writeProjetJson(json_, projetId_);
  prjManager.saveChat(projetId_, chat_);
  for (auto layer : layersImage_) {
    prjManager.saveImage(projetId_, layer.first, layer.second);
  }

    for(auto layer : layersSprite_) {
        prjManager.saveSpriteLayer(projetId_, layer.first, layer.second);
    }

    prjManager.saveSprite(projetId_, sprites_);
}

ExportDemand::ExportDemand(LiveProject &livePrj, uint projId,
                           std::string destPath, uint userId)
    : SaveTask(livePrj, projId), userId_(userId), destPath_(destPath) {}

void ExportDemand::execute(SaveWorker &worker) {
  SaveTask::execute(worker); // Saving Project
  ProjectsManager prjManager = worker.getPrjMngr();

  QByteArray data = prjManager.Zip(projetId_);
  std::string projectName = json_["name"].toString().toStdString();
  std::unique_ptr<Reponse> rps;
  rps = std::make_unique<ReponseExport>(userId_, projectName, destPath_, data);

  worker.addReponse(std::move(rps));
}
