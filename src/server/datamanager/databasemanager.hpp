#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include "memberentry.hpp"
#include "projectentry.hpp"

class DatabaseManager {
public:
    DatabaseManager();
    long long verifyLogin(const std::string& pseudo, const std::string& password);
    long long addUser(const std::string& pseudo, const std::string& password);
    bool addLink(const long long userId, const long long projectId, const int8_t role);
    bool changeRole(const long long userId, const long long projectId, const int8_t role);
    long long addProject(const std::string& name, const long long userId);
    bool updateProjectName(const long long projectId, const std::string & newName);
    long long dupProj(const std::string &newName, const long long userId);
    std::vector<MemberEntry> getProjectMembers(const long long projectId);
    std::vector<ProjectEntry> getUserProjects(const long long userId);
    std::string getPseudo(const long long userId);
    std::string getName(const long long projectId);
    int8_t getRole(const long long userId, const long long projectId);
    std::vector<ProjectEntry> getAllProjects();
    bool removeLink(const long long userId, const long long projectId);
    bool removeProject(const long long projectId);
private:
    QSqlDatabase m_db;
};
#endif
