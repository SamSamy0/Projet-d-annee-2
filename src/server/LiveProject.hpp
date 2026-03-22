#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <deque>
#include <unordered_map>

struct ModifProject {};

struct ModifProjectPixel {};

struct ModifProjectSprite {};

struct LiveProject {
    LiveProject(QJsonObject json);
    void addConnection(uint userId, uint8_t role);
    bool removeConnection(uint userId); //return true if the project is empty
    void addModif(ModifProject& modifPrj);
    QJsonObject& getJson();
    std::vector<uint>& getConnected();

    private :
    QJsonObject json_;
    std::unordered_map<uint, std::deque<ModifProject>> mapModif_;
    std::vector<uint> connectedID_;
    std::unordered_map<uint, int8_t> usersRoles_;
};