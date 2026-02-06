#include "databasemanager.h"
#include <QSqlError>
#include <QDebug>

DatabaseManager::DatabaseManager(const QString& path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    if (!m_db.open()) {
        qDebug() << "Erreur ouverture DB:" << m_db.lastError().text();
    } else { // Création de la base si elle n'existe pas
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS users ("
                   "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "pseudo TEXT UNIQUE, "
                   "password TEXT)");
    }
}

bool verifyLogin(const QString& pseudo, const QString& password) {
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE pseudo = :p AND password = :pw");
    query.bindValue(":p", pseudo);
    query.bindValue(":pw", password);

    if (query.exec() && query.next()) {
        return true;
    }
    return false;
}

//utiliser un hachage plus tard
bool registerUser(const QString& pseudo, const QString& password) {
    QSqlQuery query;
    query.prepare("INSERT INTO users (pseudo, password) VALUES (:p, ;pw)");
    query.bindValue(":p", pseudo);
    query.bindValue(":pw", password);

    if (query.exec()) {
        return true;
    }
    return false;
}
