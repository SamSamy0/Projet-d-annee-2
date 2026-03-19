#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <deque>
#include <unordered_map>

struct ModifProject {};

struct LiveProject {
    LiveProject(QJsonObject json);
    QJsonObject json_;
    std::unordered_map<uint, std::deque<ModifProject>> mapModif_;
    std::vector<uint> connectedID_;
    std::unordered_map<uint, int8_t> userRole_;
};