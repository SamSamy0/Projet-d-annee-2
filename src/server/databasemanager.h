#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

class DatabaseManager {
public:
    DatabaseManager(const QString& path);
    bool verifyLogin(const QString& pseudo, const QString& password);
    bool registerUser(const QString& pseudo, const QString& password);

private:
    QSqlDatabase m_db;
};

#endif
