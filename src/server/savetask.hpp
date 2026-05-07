#pragma once
#include "LiveProject.hpp"
#include "datamanager/projectsmanager.hpp"
#include <utility>


class SaveWorker;

class SaveTask {
    protected:
    QJsonObject json_;
    std::vector<std::pair<uint, QImage>> layersImage_;
    std::vector<std::pair<uint, QJsonObject>> layersSprite_;
    uint projetId_;
    QJsonArray chat_;
    std::map<uint, sf::Texture> sprites_;

    public:
    virtual void execute(SaveWorker& worker);
    SaveTask(LiveProject& livePrj, uint projId);

};

class ExportDemand : public SaveTask {
    uint userId_;
    public:
    ExportDemand(LiveProject& livePrj, uint projId, uint userId);
    void execute(SaveWorker& worker);
};

