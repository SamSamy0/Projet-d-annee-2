#include "databasemanager.hpp"
#include <QDebug>
#include <QSqlError>
#include <iostream>

DatabaseManager::DatabaseManager() {
  m_db = QSqlDatabase::addDatabase("QSQLITE");
  m_db.setDatabaseName("db.sqlite");
  if (!m_db.open()) {
    qDebug() << "Erreur ouverture DB:" << m_db.lastError().text();
  } else { // Pas totalement sûr mais voilou
    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON;");

        query.exec("CREATE TABLE IF NOT EXISTS users ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "pseudo TEXT UNIQUE COLLATE NOCASE CHECK(LENGTH(pseudo) BETWEEN 3 AND 15), "
                   "password TEXT)");
        query.exec("CREATE TABLE IF NOT EXISTS projects ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "name TEXT)");
        query.exec("CREATE TABLE IF NOT EXISTS links ("
                    "user_id INTEGER, "
                    "project_id INTEGER, "
                    "role INTEGER, "
                    "PRIMARY KEY (user_id, project_id), "
                    "FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE,"
                    "FOREIGN KEY (project_id) REFERENCES projects(id) ON DELETE CASCADE)");
    }
}

// renvoie l'id de l'utilisateur sinon -1
long long DatabaseManager::verifyLogin(const std::string &pseudo,
                                       const std::string &password) {
  QString qPseudo = QString::fromStdString(pseudo);
  QString qPassword = QString::fromStdString(password);

  QSqlQuery query;
  query.prepare("SELECT id FROM users WHERE pseudo = :p AND password = :pw");
  query.bindValue(":p", qPseudo);
  query.bindValue(":pw", qPassword);

  if (query.exec() && query.next()) {
    return query.value(0).toLongLong();
  }
  return -1;
}

// utiliser un hachage plus tard
long long DatabaseManager::addUser(const std::string &pseudo,
                                   const std::string &password) {
  QString qPseudo = QString::fromStdString(pseudo);
  QString qPassword = QString::fromStdString(password);

  QSqlQuery query;
  query.prepare("INSERT INTO users (pseudo, password) VALUES (:p, :pw)");
  query.bindValue(":p", qPseudo);
  query.bindValue(":pw", qPassword);

  if (query.exec()) {
    return query.lastInsertId().toLongLong();
  }
  qDebug() << "Erreur d'inscription :" << query.lastError().text();
  return -1;
}

bool DatabaseManager::addLink(const long long userId, const long long projectId,
                              const int8_t role) {
  QSqlQuery query;
  query.prepare(
      "INSERT INTO links (user_id, project_id, role) VALUES (:u, :p, :r)");

  query.bindValue(":u", static_cast<qlonglong>(userId));
  query.bindValue(":p", static_cast<qlonglong>(projectId));
  query.bindValue(":r", static_cast<int>(role));

  if (query.exec()) {
    return true;
  }
  qDebug() << "Erreur de lien (addLink) :" << query.lastError().text();

  return false;
}

long long DatabaseManager::addProject(const std::string &name,
                                      const long long userId) {
  QString qName = QString::fromStdString(name);

  QSqlQuery query;
  query.prepare("INSERT INTO projects (name) VALUES (:n)");
  query.bindValue(":n", qName);

  if (query.exec()) {
    long long projId = query.lastInsertId().toLongLong();
    this->addLink(userId, projId, 2);
    return projId;
  }
  qDebug() << "Erreur d'inscription :" << query.lastError().text();
  return -1;
}

bool DatabaseManager::updateProjectName(const long long projectId,
                                        const std::string &newName) {
  QSqlQuery query;
  query.prepare("Update projects SET name = :n WHERE id = :i");
  query.bindValue(":n", QString::fromStdString(newName));
  query.bindValue(":i", static_cast<qlonglong>(projectId));
  if (query.exec()) {
    return query.numRowsAffected() > 0;
  }
  qDebug() << "Erreur DatabaseManager (updateProjectName):"
           << query.lastError().text();
  return false;
};
// Returns the Id of the new duplicate Project
long long DatabaseManager::dupProj(const std::string &newName,
                                   const long long userId) {
  QSqlQuery query;
  query.prepare("INSERT INTO projects (name) VALUES (:n)");
  query.bindValue(":n", QString::fromStdString(newName));
  if (query.exec()) {
    long long newId = query.lastInsertId().toLongLong();
    this->addLink(userId, newId, 2);
    return newId;
  }

  return -1;
}

bool DatabaseManager::changeRole(const long long userId,
                                 const long long projectId, const int8_t role) {
  QSqlQuery query;

  query.prepare(
      "UPDATE links SET role = :r WHERE user_id = :u AND project_id = :p");
  query.bindValue(":r", static_cast<int>(role));
  query.bindValue(":u", static_cast<qlonglong>(userId));
  query.bindValue(":p", static_cast<qlonglong>(projectId));

  if (query.exec()) {
    return query.numRowsAffected() > 0;
  }
  qDebug() << "Erreur de lien (changeRole):" << query.lastError().text();
  return false;
}

std::vector<MemberEntry>
DatabaseManager::getProjectMembers(long long projectId) {
  std::vector<MemberEntry> members;
  QSqlQuery query;

  query.prepare("SELECT user_id, role FROM links WHERE project_id = :p");
  query.bindValue(":p", projectId);

  if (query.exec()) {
    while (query.next()) {
      MemberEntry m;
      m.userId = query.value(0).toLongLong();
      m.role = static_cast<int8_t>(query.value(1).toInt());
      m.pseudo = getPseudo(m.userId);
      members.push_back(m);
    }
  }
  return members;
}

std::vector<ProjectEntry> DatabaseManager::getUserProjects(long long userId) {
  std::vector<ProjectEntry> projects;
  QSqlQuery query;

  query.prepare("SELECT project_id, role FROM links WHERE user_id = :u");
  query.bindValue(":u", userId);

  if (query.exec()) {
    while (query.next()) {
      ProjectEntry p;
      p.projectId = query.value(0).toLongLong();
      p.role = static_cast<int8_t>(query.value(1).toInt());
      p.name = getName(p.projectId);
      projects.push_back(p);
    }
  }
  return projects;
}

std::string DatabaseManager::getPseudo(const long long userId) {
  QSqlQuery query;

  query.prepare("SELECT pseudo FROM users WHERE id = :u");
  query.bindValue(":u", userId);

  if (!query.exec()) {
    qDebug() << "Erreur SQL (getPseudo) :" << query.lastError().text();
    return "";
  }
  if (query.next()) {
    return query.value(0).toString().toStdString();
  } else {
    qDebug() << "Avertissement : Aucun utilisateur trouvé pour l'ID" << userId;
    return "";
  }
}

std::string DatabaseManager::getName(const long long projectId) {
  QSqlQuery query;

  query.prepare("SELECT name FROM projects WHERE id = :p");
  query.bindValue(":p", projectId);

  if (!query.exec()) {
    qDebug() << "Erreur SQL (getName) :" << query.lastError().text();
    return "";
  }
  if (query.next()) {
    return query.value(0).toString().toStdString();
  } else {
    qDebug() << "Avertissement : Aucun projet trouvé pour l'ID" << projectId;
    return "";
  }
}

int8_t DatabaseManager::getRole(const long long userId,
                                const long long projectId) {
  QSqlQuery query;

  query.prepare(
      "SELECT role FROM links WHERE user_id = :u AND project_id = :p");
  query.bindValue(":u", userId);
  query.bindValue(":p", projectId);

  if (!query.exec()) {
    qDebug() << "Erreur SQL (getRole) :" << query.lastError().text();
    return -1;
  }
  if (query.next()) {
    return static_cast<int8_t>(query.value(0).toInt());
  } else {
    qDebug() << "Avertissement : Aucun projet trouvé pour l'ID" << projectId;
    return -1;
  }
}

std::vector<ProjectEntry> DatabaseManager::getAllProjects() {
  std::vector<ProjectEntry> projects;
  QSqlQuery query;

  query.prepare("SELECT id, name FROM projects");

  if (query.exec()) {
    while (query.next()) {
      ProjectEntry p;
      p.projectId = query.value(0).toLongLong();
      p.role = static_cast<int8_t>(2);
      p.name = query.value(1).toString().toStdString();
      projects.push_back(p);
    }
  }
  return projects;
}

bool DatabaseManager::removeLink(const long long userId, const long long projectId) {
    QSqlQuery query;
    query.prepare("DELETE FROM links WHERE user_id = :uId AND project_id = :pId");
    query.bindValue(":uId", userId);
    query.bindValue(":pId", projectId);

    if (!query.exec()) {
        qDebug() << "Erreur suppression lien:" << query.lastError().text();
        return false;
    }
    return true;
}

bool DatabaseManager::removeProject(const long long projectId) {
    QSqlQuery query;
    query.prepare("DELETE FROM projects WHERE id = :pId");
    query.bindValue(":pId", projectId);

    if (!query.exec()) {
        qDebug() << "Erreur suppression projet:" << query.lastError().text();
        return false;
    }
    return true;
}

