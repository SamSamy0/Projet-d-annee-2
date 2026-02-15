#include "projectsmanager.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QFileInfo>
#include <QDebug>

ProjectsManager::ProjectsManager(const std::string &rootPath)
    : m_rootPath(QString::fromStdString(rootPath))
{
    QDir dir;
    if (!dir.exists(m_rootPath)) {
        dir.mkpath(m_rootPath);
    }
}

bool ProjectsManager::saveProjectJson(int id, const QJsonObject &data) {
    if (!ensureDirectoryExists(id)) {
        return false;
    }

    QString filePath = getProjectPath(id) + "/donnees.json";
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument doc(data);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        return true;
    }

    qCritical() << "Impossible d'ouvrir le fichier en écriture :" << filePath;
    return false;
}

bool ProjectsManager::saveImage(int id, const QString &fileName, const QByteArray &data) {
    if (!ensureDirectoryExists(id)) return false;

    QString destPath = getProjectPath(id) + "/images/" + fileName;

    QFile file(destPath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(data);
        file.close();
        return true;
    }

    qCritical() << "Impossible d'écrire le média reçu pour le projet" << id;
    return false;
}

QString ProjectsManager::getProjectPath(int id) const {
    return m_rootPath + "/project_" + QString::number(id);
}

bool ProjectsManager::ensureDirectoryExists(int id) const {
    QString path = getProjectPath(id);
    QDir dir;

    if (!dir.mkpath(path + "/images")) {
        qCritical() << "Erreur de création du dossier projet pour l'ID :" << id;
        return false;
    }
    return true;
}
