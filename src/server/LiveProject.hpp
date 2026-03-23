#pragma once
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <deque>
#include <unordered_map>
#include <QImage>

enum class LayerType : uint8_t {
    Pixel,
    Sprit,
};

struct LiveProject {
    LiveProject(QJsonObject json);
    LiveProject(uint id, const QString &projectName, uint width, uint height, uint scale);
    void addConnection(uint userId, uint8_t role);
    bool removeConnection(uint userId); //return true if the project is empty
    QJsonObject& getJson();
    std::vector<uint>& getConnected();
    void addLayerImage(uint layerId, const QImage& img);

    std::unordered_map<uint, QImage> layersImage_;

    uint getScale();

    private :
    QJsonObject json_;
    std::vector<uint> connectedID_;
    std::unordered_map<uint, int8_t> usersRoles_;
    std::unordered_map<uint, LayerType> layersTypes_;
};