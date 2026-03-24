#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "memberentry.hpp"
#include "projectentry.hpp"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

class DatabaseManager {
public:
  DatabaseManager();
  uint verifyLogin(const std::string &pseudo, const std::string &password);
  uint addUser(const std::string &pseudo, const std::string &password);
  bool addLink(const uint userId, const uint projectId, const int8_t role);
  bool changeRole(const uint userId, const uint projectId, const int8_t role);
  uint addProject(const std::string &name, const uint userId);
  bool updateProjectName(const uint projectId, const std::string &newName);
  uint dupProj(const std::string &newName, const uint userId);
  std::vector<MemberEntry> getProjectMembers(const uint projectId);
  std::vector<ProjectEntry> getUserProjects(const uint userId);
  std::string getPseudo(const uint userId);
  std::string getName(const uint projectId);
  int8_t getRole(const uint userId, const uint projectId);
  std::vector<ProjectEntry> getAllProjects();
  bool removeLink(const uint userId, const uint projectId);
  bool removeProject(const uint projectId);
  bool saveToken(std::string token, uint8_t role, int projectId, bool isUsed);
  // bool saveToken(std::string token, uint8_t role, int projectId);
  bool checkToken(uint userId, std::string token);

private:
  QSqlDatabase m_db;
};
#endif
