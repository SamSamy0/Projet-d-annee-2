#include "projectsmanager.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QFileInfo>
#include <QDebug>
#include <QJsonArray>

ProjectsManager::ProjectsManager(const std::string &rootPath)
    : m_rootPath(QString::fromStdString(rootPath))
{
    QDir dir;
    if (!dir.exists(m_rootPath)) {
        dir.mkpath(m_rootPath);
    }
}

bool ProjectsManager::createProjectJson(int id, const QString &projectName, int width, int height, uint scale) {
    if (!ensureDirectoryExists(id)) {
        return false;
    }
    QJsonObject root;
    QJsonArray emptylayers;
    
    root["id"] = id;
    root["name"] = projectName;
    root["width"] = width;
    root["height"] = height;
    root["scale"] = static_cast<double>(scale);

    root["layers"] = emptylayers;

    QString filePath = getProjectPath(id) + "/donnees.json";
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument doc(root);
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

bool ProjectsManager::updateProjectName(int id, const QString& newName){
    QJsonObject root = loadProjectJson(id);
    if (root.isEmpty())return false;
    root["name"] = newName;
    QString filePath = getProjectPath(id) + "/donnees.json";
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument doc(root);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        return true;
    }

    qCritical() << "Impossible d'ouvrir le fichier en écriture :" << filePath;
    return false;
    
}

bool ProjectsManager::copyProjectFolder(int oldId, int newId){
    QString srcPath = getProjectPath(oldId);
    QString destinationPath = getProjectPath(newId);
    return copyRecursively(srcPath, destinationPath);
}

// bool ProjectsManager::copyRecursively(const QString& srcPath, const QString& destinationPath){
//     QDir dir(srcPath);
//     if (! dir.exists())
//         return false ;
//     QDir dstDir;
//     dstDir.mkpath(destinationPath);
//     //Copy directory
//     foreach (QString dirName, dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
//         QFile::copy(srcPath + "/" + dirName , destinationPath + "/" + dirName);
//     }
//     //Copy fils in directory and subdirectory
//     foreach (QString fileName, dir.entryList(QDir::Files)) {
//         QFile::copy(srcPath + "/" + fileName, destinationPath + "/" + fileName);
//     }
//     return true;
// }
bool ProjectsManager::copyRecursively(const QString &srcFilePath,
                            const QString &tgtFilePath)
{
    QFileInfo srcFileInfo(srcFilePath);
    if (srcFileInfo.isDir()) {
        QDir targetDir(tgtFilePath);
        targetDir.cdUp();
        if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
            return false;
        QDir sourceDir(srcFilePath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach (const QString &fileName, fileNames) {
            const QString newSrcFilePath
                    = srcFilePath + QLatin1Char('/') + fileName;
            const QString newTgtFilePath
                    = tgtFilePath + QLatin1Char('/') + fileName;
            if (!copyRecursively(newSrcFilePath, newTgtFilePath))
                return false;
        }
    } else {
        if (!QFile::copy(srcFilePath, tgtFilePath))
            return false;
    }
    return true;
}
bool ProjectsManager::updateJsonDup(int newId, const QString& newName){
    QJsonObject root = loadProjectJson(newId);
    if (root.isEmpty())return false;
    root["name"] = newName;
    root["id"] = newId;
    QString filePath = getProjectPath(newId) + "/donnees.json";
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument doc(root);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        return true;
    }

    qCritical() << "Impossible d'ouvrir le fichier en écriture :" << filePath;
    return false;
    
}


QJsonObject ProjectsManager::loadProjectJson(int id) {
    QString filePath = getProjectPath(id) + "/donnees.json";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Impossible d'ouvrir le fichier projet pour l'ID" << id << ":" << filePath;
        return QJsonObject();
    }

    QByteArray rawData = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(rawData, &error);

    if (error.error != QJsonParseError::NoError) {
        qCritical() << "Erreur de parsing JSON pour le projet" << id << ":" << error.errorString();
        return QJsonObject();
    }

    if (!doc.isObject()) {
        qCritical() << "Le contenu du fichier n'est pas un objet JSON valide pour le projet" << id;
        return QJsonObject();
    }

    return doc.object();
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
