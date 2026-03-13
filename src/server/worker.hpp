#ifndef WORKER_HPP
#define WORKER_HPP

#include <atomic>
#include "mutexqueue.hpp"
#include "message/message.hpp"
#include "datamanager/databasemanager.hpp"
#include "datamanager/projectsmanager.hpp"
#include "reponse/reponse.hpp"

class Worker {
public:
    explicit Worker(MutexQueue<IMessage>& d_queue, MutexQueue<Reponse>& r_queue);
    ~Worker() = default;
    void run();
    void stop();
    void pushNetwork(std::unique_ptr<Reponse> rps);

    long long verifyLogin(const std::string& pseudo, const std::string& password);
    long long addUser(const std::string& pseudo, const std::string& password);
    bool addLink(const long long userId, const long long projectId, const int8_t role);
    bool changeRole(const long long userId, const long long projectId, const int8_t role);
    long long addProjectSQL(const std::string& name, const long long userId);
    std::vector<MemberEntry> getProjectMembers(const long long projectId);
    std::vector<ProjectEntry> getUserProjects(const long long userId);
    int8_t getRole(const long long userId, const long long projectId);
    std::vector<ProjectEntry> getAllProjects();
    bool renameProject(int projectId, const std::string& newName);

    bool createProjectJson(int id, const std::string &projectName, int width, int height, uint scale);
    QJsonObject loadProjectJson(int id);
    bool saveImage(int id, const std::string &fileName, const QByteArray &data);


private:
    std::atomic<bool> mRunning_;
    MutexQueue<IMessage>& demQueue_;
    MutexQueue<Reponse>& repQueue_;
    DatabaseManager dbManager_;
    ProjectsManager projManager_;
};

#endif
