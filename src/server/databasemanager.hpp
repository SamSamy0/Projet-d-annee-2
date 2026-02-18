#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

struct MemberEntry {
    long long userId;
    int8_t role;
    std::string pseudo;
};

struct ProjectEntry {
    long long projectId;
    int8_t role;
    std::string name;
};

class DatabaseManager {
public:
    DatabaseManager();
    long long verifyLogin(const std::string& pseudo, const std::string& password);
    long long addUser(const std::string& pseudo, const std::string& password);
    bool addLink(const long long userId, const long long projectId, const int8_t role);
    bool changeRole(const long long userId, const long long projectId, const int8_t role);
    long long addProject(const std::string& name, const long long userId);
    std::vector<MemberEntry> getProjectMembers(const long long projectId);
    std::vector<ProjectEntry> getUserProjects(const long long userId);
    std::string getPseudo(const long long userId);
    std::string getName(const long long projectId);
    int8_t getRole(const long long userId, const long long projectId);
    std::vector<ProjectEntry> getAllProjects();
private:
    QSqlDatabase m_db;
};
#endif
