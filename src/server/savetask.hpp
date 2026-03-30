#pragma once
#include "LiveProject.hpp"
#include "datamanager/projectsmanager.hpp"
#include <utility>

class SaveTask {
    QJsonObject json_;
    std::vector<std::pair<uint, QImage>> layersImage_;
    std::vector<std::pair<uint, std::vector<Sprite>>> layersSprite_;
    uint projetId_;

    public:
    void execute(ProjectsManager prjManager);
    SaveTask(LiveProject& livePrj, uint projId);

};

