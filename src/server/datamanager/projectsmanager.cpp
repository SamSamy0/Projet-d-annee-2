#include "projectsmanager.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QFileInfo>
#include <QDebug>
#include <QJsonArray>
#include <QImage>
#include <QColor>
#include <iostream>
#include <QBuffer>
#include <QSaveFile>


ProjectsManager::ProjectsManager(const std::string &rootPath)
    : rootPath_(QString::fromStdString(rootPath))
{
    QDir dir;
    if (!dir.exists(rootPath_)) {
        dir.mkpath(rootPath_);
    }
    if(!dir.exists(rootPath_ + "/chats")){
        dir.mkpath(rootPath_ + "/chats");
    }
}

/*bool ProjectsManager::createProjectJson(uint id, const QString &projectName, uint width, uint height, uint scale) {
    if (!ensureDirectoryExists(id)) {
        return false;
    }
    QJsonObject root;
    QJsonObject firstLayer;
    QJsonArray emptylayers;
    
    root["id"] = static_cast<int>(id);
    root["name"] = projectName;
    root["width"] =  static_cast<int>(width);
    root["height"] =  static_cast<int>(height);
    root["scale"] = static_cast<int>(scale);
    root["layerId"] = (int)1;

    firstLayer["type"] = "pixel";
    firstLayer["id"] = 0;
    firstLayer["x"] = 0;
    firstLayer["y"] = 0;

    emptylayers.append(firstLayer);

    root["layers"] = emptylayers;

    QString filePath = getProjectPath(id) + "/donnees.json";
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument doc(root);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        addCalquePixel(id, width, height, 0);
        return true;
    }

    qCritical() << "Impossible d'ouvrir le fichier en écriture :" << filePath;
    return false;
}*/

bool ProjectsManager::writeProjetJson(QJsonObject& jsonObject, uint id) {

    ensureDirectoryExists(id);

    QString filePath = getProjectPath(id) + "/donnees.json";
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument doc(jsonObject);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        return true;
    }

    qCritical() << "Impossible d'ouvrir le fichier en écriture :" << filePath;
    return false;
}

bool ProjectsManager::saveSprite(uint id, uint layerId, QJsonObject& jsonObject) {

    ensureDirectoryExists(id);

    QString filePath = getProjectPath(id) + "/images/" + QString::number(layerId) + ".json";
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QJsonDocument doc(jsonObject);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        return true;
    }

    qCritical() << "Impossible d'ouvrir le fichier en écriture :" << filePath;
    return false;
}

QJsonObject ProjectsManager::loadSprite(uint id, uint layerId) {
    QString filePath = getProjectPath(id) + "/images/" + QString::number(layerId) + ".json";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Impossible d'ouvrir le fichier layer pour l'ID" << layerId << ":" << filePath;
        return QJsonObject();
    }

    QByteArray rawData = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(rawData, &error);

    if (error.error != QJsonParseError::NoError) {
        qCritical() << "Erreur de parsing JSON pour le layer" << layerId << ":" << error.errorString();
        return QJsonObject();
    }

    if (!doc.isObject()) {
        qCritical() << "Le contenu du fichier n'est pas un objet JSON valide pour le layer" << layerId;
        return QJsonObject();
    }

    return doc.object();
}

/*QByteArray imageToPng(const QImage& image) {
    QByteArray ba;

    QBuffer buffer(&ba);
    
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");

    return ba;
}*/

bool ProjectsManager::saveImage(uint id, uint imageId, const QImage &data) {
    if (!ensureDirectoryExists(id)) return false;

    QString destPath = getProjectPath(id) + "/images/" + QString::number(imageId) + ".png";


    
    QSaveFile file(destPath);
    if (file.open(QIODevice::WriteOnly)) {
        if (data.save(&file, "PNG")) {
            return file.commit();
        }
    }

    qCritical() << "Impossible d'écrire le média reçu pour le projet" << id;
    return false;
}

QImage ProjectsManager::loadImage(uint projectId, uint layerId) {
    QString path = getProjectPath(projectId) + "/images/" + QString::number(layerId) + ".png";

    QImage img;
    if (!img.load(path, "PNG")) {
        qCritical() << "[ProjectsManager] Impossible de charger l'image :" << path;
        return QImage(); 
    }

    if (img.format() != QImage::Format_ARGB32) {
        img = img.convertToFormat(QImage::Format_ARGB32);
    }

    return img;
}

bool ProjectsManager::updateProjectName(uint id, const QString& newName){
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

bool ProjectsManager::copyProjectFolder(uint oldId, uint newId){
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


bool ProjectsManager::updateJsonDup(uint newId, const QString& newName){
    QJsonObject root = loadProjectJson(newId);
    if (root.isEmpty())return false;
    root["name"] = newName;
    root["id"] = static_cast<int>(newId);
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


QJsonObject ProjectsManager::loadProjectJson(uint id) {
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

QString ProjectsManager::getProjectPath(uint id) const {
    return rootPath_ + "/project_" + QString::number(id);
}

bool ProjectsManager::ensureDirectoryExists(uint id) const {
    QString path = getProjectPath(id);
    QDir dir;

    if (!dir.mkpath(path + "/images")) {
        qCritical() << "Erreur de création du dossier projet pour l'ID :" << id;
        return false;
    }
    return true;
}

bool ProjectsManager::deleteProject(uint id) {
    QString path = getProjectPath(id);
    QDir dir(path);

    if (dir.exists())

    if (dir.removeRecursively()) {
        return true;
    }
    qCritical() << "Erreur de suppression de projet pour l'ID:" << id;
    return false;
}

bool ProjectsManager::saveChat(uint projectId, QJsonArray& chat) {

    QString destPath = rootPath_ + "/chats/chat_" + QString::number(projectId) + ".json";
    
    QSaveFile file(destPath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(chat);
        file.write(doc.toJson(QJsonDocument::Indented));
        return file.commit();
    }

    qCritical() << "Impossible d'écrire le chat pour le projet" << projectId;
    return false;
}

QJsonArray ProjectsManager::loadChat(uint projectId) {
    QString filePath = rootPath_ + "/chats/chat_" + QString::number(projectId) + ".json";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Impossible d'ouvrir le fichier de chat pour l'ID" << projectId << ":" << filePath;
        return QJsonArray();
    }

    QByteArray rawData = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(rawData, &error);

    if (error.error != QJsonParseError::NoError) {
        qCritical() << "Erreur de parsing JSON pour le chat du projet" << projectId << ":" << error.errorString();
        return QJsonArray();
    }

    if (!doc.isArray()) {
        qCritical() << "Le contenu du fichier n'est pas un tableau JSON valide pour le chat du projet" << projectId;
        return QJsonArray();
    }

    return doc.array();
}