#include "savetask.hpp"
#include "saveworker.hpp"
#include "reponse/reponse.hpp"

SaveTask::SaveTask(LiveProject& livePrj, uint projId) 
    : json_(livePrj.getJson()), projetId_(projId), chat_(livePrj.getChatJson())
{
    const std::unordered_map<uint, QImage>& layersImagePrj = livePrj.getImageMap();
    for (auto it = layersImagePrj.begin(); it != layersImagePrj.end(); ++it) {
        layersImage_.emplace_back(it->first, it->second);
    }

    const std::unordered_map<uint, SpriteLayer>& layersSpritePrj = livePrj.getSpritesMap();
    for (auto it = layersSpritePrj.begin(); it != layersSpritePrj.end(); ++it) {
        layersSprite_.emplace_back(it->first, it->second.load());
    }
}

void SaveTask::execute(SaveWorker& worker) {
    ProjectsManager prjManager = worker.getPrjMngr();
    prjManager.writeProjetJson(json_, projetId_);
    prjManager.saveChat(projetId_, chat_);
    for (auto layer : layersImage_) {
        prjManager.saveImage(projetId_, layer.first, layer.second);
    }

    for(auto layer : layersSprite_) {
        prjManager.saveSprite(projetId_, layer.first, layer.second);
    }
}

ExportDemand::ExportDemand(LiveProject& livePrj, uint projId, uint userId) : 
SaveTask(livePrj, projId), userId_(userId) {}

void ExportDemand::execute(SaveWorker& worker) {
    SaveTask::execute(worker); //sauvegarde du projet omg;
    ProjectsManager prjManager = worker.getPrjMngr();

    QByteArray data = prjManager.Zip(projetId_);

    std::unique_ptr<Reponse> rps;
    rps = std::make_unique<ReponseExport>(userId_, data);

    std::cout << "export envoyé " << std::endl;
    worker.addReponse(std::move(rps));
}