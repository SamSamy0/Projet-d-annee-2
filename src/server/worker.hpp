#ifndef WORKER_HPP
#define WORKER_HPP

#include <atomic>
#include "mutexqueue.hpp"
#include "message/message.hpp"
#include "datamanager/databasemanager.hpp"
#include "datamanager/projectsmanager.hpp"
#include "reponse/reponse.hpp"
#include "LiveProject.hpp"

class Worker {
public:
    explicit Worker(MutexQueue<IMessage>& d_queue, MutexQueue<Reponse>& r_queue);
    ~Worker() = default;
    void run();
    void stop();
    void pushNetwork(std::unique_ptr<Reponse> rps);

    uint verifyLogin(const std::string& pseudo, const std::string& password);
    uint addUser(const std::string& pseudo, const std::string& password);
    bool addLink(const uint userId, const uint projectId, const uint8_t role);
    bool changeRole(const uint userId, const uint projectId, const uint8_t role);
    uint addProjectSQL(const std::string& name, const uint userId);
    std::vector<MemberEntry> getProjectMembers(const uint projectId);
    std::vector<ProjectEntry> getUserProjects(const uint userId);
    uint8_t getRole(const uint userId, const uint projectId);
    std::vector<ProjectEntry> getAllProjects();
    bool renameProject(uint projectId, const std::string& newName);
    uint duplicateProject(uint oldId, const std::string& newName, uint userId);
    bool deleteProject(uint projectId);

    bool createProjectJson(uint id, const std::string &projectName, uint width, uint height, uint scale);
    bool writeProjetJson(QJsonObject& jsonObject, uint id);
    QJsonObject loadProjectJson(uint id);
    bool saveImage(uint id, const std::string &fileName, const QByteArray &data);
    QByteArray getByteJson(uint projetId);

    std::unordered_map<uint, LiveProject> mapProjet_;


private:
    std::atomic<bool> mRunning_;
    MutexQueue<IMessage>& demQueue_;
    MutexQueue<Reponse>& repQueue_;
    DatabaseManager dbManager_;
    ProjectsManager projManager_;
    
};

#endif
