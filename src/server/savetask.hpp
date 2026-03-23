#pragma once
#include "LiveProject.hpp"
#include "datamanager/projectsmanager.hpp"
#include <utility>

class SaveTask {
    QJsonObject json_;
    std::vector<std::pair<uint, QImage>> layersImage_;
    uint projetId_;

    public:
    void execute(ProjectsManager prjManager);
    SaveTask(uint projId, QJsonObject json, const std::unordered_map<uint, QImage>& layerImage);

};

