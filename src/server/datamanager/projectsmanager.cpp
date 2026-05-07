#include "projectsmanager.hpp"
#include "miniz.h"
#include <QBuffer>
#include <QColor>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSaveFile>
#include <iostream>
#include <string>
#include <SFML/Graphics/Image.hpp>

ProjectsManager::ProjectsManager(const std::string &rootPath)
    : rootPath_(QString::fromStdString(rootPath)) {
  QDir dir;
  if (!dir.exists(rootPath_)) {
    dir.mkpath(rootPath_);
  }
  if (!dir.exists(rootPath_ + "/chats")) {
    dir.mkpath(rootPath_ + "/chats");
  }
  if (!dir.exists(rootPath_ + "/projectZipped")) {
    dir.mkpath(rootPath_ + "/projectZipped");
  }
  /*
  if (!dir.exists("../export")) {
    dir.mkpath("../export");
  }
  if (!dir.exists("../import")) {
    dir.mkpath("../import");
  }
  if (!dir.exists("../export_image")) {
    dir.mkpath("../export_image");
  }*/
}

/*bool ProjectsManager::createProjectJson(uint id, const QString &projectName,
uint width, uint height, uint scale) { if (!ensureDirectoryExists(id)) { return
false;
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

bool ProjectsManager::writeProjetJson(QJsonObject &jsonObject, uint id) {
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

bool ProjectsManager::saveSpriteLayer(uint id, uint layerId,
                                 QJsonObject &jsonObject) {
  ensureDirectoryExists(id);

  QString filePath =
      getProjectPath(id) + "/images/" + QString::number(layerId) + ".json";
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

QJsonObject ProjectsManager::loadSpriteLayer(uint id, uint layerId) {
  QString filePath =
      getProjectPath(id) + "/images/" + QString::number(layerId) + ".json";
  QFile file(filePath);

  if (!file.open(QIODevice::ReadOnly)) {
    qWarning() << "Impossible d'ouvrir le fichier layer pour l'ID" << layerId
               << ":" << filePath;
    return QJsonObject();
  }

  QByteArray rawData = file.readAll();
  file.close();

  QJsonParseError error;
  QJsonDocument doc = QJsonDocument::fromJson(rawData, &error);

  if (error.error != QJsonParseError::NoError) {
    qCritical() << "Erreur de parsing JSON pour le layer" << layerId << ":"
                << error.errorString();
    return QJsonObject();
  }

  if (!doc.isObject()) {
    qCritical()
        << "Le contenu du fichier n'est pas un objet JSON valide pour le layer"
        << layerId;
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
  if (!ensureDirectoryExists(id))
    return false;

  QString destPath =
      getProjectPath(id) + "/images/" + QString::number(imageId) + ".png";

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
  QString path = getProjectPath(projectId) + "/images/" +
                 QString::number(layerId) + ".png";

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

// NOTE: mettre la fonction zip ici et mettre le zip dans QFile pour que le
// worker envoie ensuite dans un package sfml
// Je lis mon binaire en fichier qui va etre construit sur mon disque dur
bool ProjectsManager::updateProjectName(uint id, const QString &newName) {
  QJsonObject root = loadProjectJson(id);
  if (root.isEmpty())
    return false;
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

bool ProjectsManager::copyProjectFolder(uint oldId, uint newId) {
  QString srcPath = getProjectPath(oldId);
  QString destinationPath = getProjectPath(newId);
  return copyRecursively(srcPath, destinationPath);
}

// bool ProjectsManager::copyRecursively(const QString& srcPath, const QString&
// destinationPath){
//     QDir dir(srcPath);
//     if (! dir.exists())
//         return false ;
//     QDir dstDir;
//     dstDir.mkpath(destinationPath);
//     //Copy directory
//     foreach (QString dirName, dir.entryList(QDir::Dirs |
//     QDir::NoDotAndDotDot)) {
//         QFile::copy(srcPath + "/" + dirName , destinationPath + "/" +
//         dirName);
//     }
//     //Copy fils in directory and subdirectory
//     foreach (QString fileName, dir.entryList(QDir::Files)) {
//         QFile::copy(srcPath + "/" + fileName, destinationPath + "/" +
//         fileName);
//     }
//     return true;
// }
bool ProjectsManager::copyRecursively(const QString &srcFilePath,
                                      const QString &tgtFilePath) {
  QFileInfo srcFileInfo(srcFilePath);
  if (srcFileInfo.isDir()) {
    QDir targetDir(tgtFilePath);
    targetDir.cdUp();
    if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
      return false;
    QDir sourceDir(srcFilePath);
    QStringList fileNames =
        sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot |
                            QDir::Hidden | QDir::System);
    foreach (const QString &fileName, fileNames) {
      const QString newSrcFilePath = srcFilePath + QLatin1Char('/') + fileName;
      const QString newTgtFilePath = tgtFilePath + QLatin1Char('/') + fileName;
      if (!copyRecursively(newSrcFilePath, newTgtFilePath))
        return false;
    }
  } else {
    if (!QFile::copy(srcFilePath, tgtFilePath))
      return false;
  }
  return true;
}

bool ProjectsManager::updateJsonDup(uint newId, const QString &newName) {
  QJsonObject root = loadProjectJson(newId);
  if (root.isEmpty())
    return false;
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
    qWarning() << "Impossible d'ouvrir le fichier projet pour l'ID" << id << ":"
               << filePath;
    return QJsonObject();
  }

  QByteArray rawData = file.readAll();
  file.close();

  QJsonParseError error;
  QJsonDocument doc = QJsonDocument::fromJson(rawData, &error);

  if (error.error != QJsonParseError::NoError) {
    qCritical() << "Erreur de parsing JSON pour le projet" << id << ":"
                << error.errorString();
    return QJsonObject();
  }

  if (!doc.isObject()) {
    qCritical()
        << "Le contenu du fichier n'est pas un objet JSON valide pour le projet"
        << id;
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

  if (!dir.mkpath(path + "/sprites")) {
    qCritical() << "Erreur de création du dossier sprites pour l'ID :" << id;
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

bool ProjectsManager::saveChat(uint projectId, QJsonArray &chat) {
  QString destPath =
      rootPath_ + "/chats/chat_" + QString::number(projectId) + ".json";

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
  QString filePath =
      rootPath_ + "/chats/chat_" + QString::number(projectId) + ".json";
  QFile file(filePath);

  if (!file.open(QIODevice::ReadOnly)) {
    qWarning() << "Impossible d'ouvrir le fichier de chat pour l'ID"
               << projectId << ":" << filePath;
    return QJsonArray();
  }

  QByteArray rawData = file.readAll();
  file.close();

  QJsonParseError error;
  QJsonDocument doc = QJsonDocument::fromJson(rawData, &error);

  if (error.error != QJsonParseError::NoError) {
    qCritical() << "Erreur de parsing JSON pour le chat du projet" << projectId
                << ":" << error.errorString();
    return QJsonArray();
  }

  if (!doc.isArray()) {
    qCritical() << "Le contenu du fichier n'est pas un tableau JSON valide "
                   "pour le chat du projet"
                << projectId;
    return QJsonArray();
  }

  return doc.array();
}

QByteArray ProjectsManager::Zip(uint projectId) {
  std::string projectIdStr = std::to_string(projectId);

  std::string projectBasePath =
      rootPath_.toStdString() + "/project_" + projectIdStr;
  std::string zipPath = rootPath_.toStdString() + "/projectZipped/project_" +
                        projectIdStr + ".natif";

  // Creating zip file
  mz_zip_archive zip_archive;
  mz_zip_zero_struct(&zip_archive);
  // Filling zip with 0
  mz_zip_writer_init_file(&zip_archive, zipPath.c_str(), 0);

  // Writing in zipFile png
  try {
    for (const auto &entry :
         std::filesystem::directory_iterator(projectBasePath)) {
      std::string entryName = entry.path().filename().string();
      std::string fullPathOnDisk = entry.path().string();

      // Parsing files
      if (entry.is_directory()) {
        if (entryName == "images") {
          for (const auto &subEntry :
               std::filesystem::recursive_directory_iterator(entry.path())) {
            // les fichiers
            std::string subFullPath = subEntry.path().string();

            // Creating relative path for zip (ex: "images/0.png")
            std::string relativePath =
                std::filesystem::relative(subEntry.path(), projectBasePath)
                    .string();

            mz_zip_writer_add_file(&zip_archive, relativePath.c_str(),
                                   subFullPath.c_str(), NULL, 0,
                                   MZ_BEST_COMPRESSION);
          }
        }
      } else if (entry.is_regular_file()) {
        if (entryName == "donnees.json") {
          mz_zip_writer_add_file(&zip_archive, entryName.c_str(),
                                 fullPathOnDisk.c_str(), NULL, 0,
                                 MZ_BEST_COMPRESSION);
        }
      }
    }
  } catch (const std::exception &e) {
    std::cerr << "Erreur système lors du parcours du dossier : " << e.what()
              << std::endl;
  }

  // Cleaning and closing zip
  mz_zip_writer_finalize_archive(&zip_archive);
  mz_zip_writer_end(&zip_archive);
  std::cout << "fin de la compression " << std::endl;

  QString Qpath = QString::fromStdString(zipPath);
  QFile fileZip(Qpath);

  if (!fileZip.open(QIODevice::ReadOnly)) {
    qWarning() << "Impossible d'ouvrir le fichier projet pour l'ID" << projectId
               << ":" << Qpath;
    return QByteArray();
  }

  QByteArray rawData = fileZip.readAll();
  fileZip.close();

  // Sending Bytes to client
  return rawData;
}

bool ProjectsManager::saveSprite(uint projectId, std::map<uint, sf::Texture> &sprites) {
  std::string projectPath = getProjectPath(projectId).toStdString();
  std::string spritesDir = projectPath + "/sprites/";

  if (!std::filesystem::exists(spritesDir)) {
      std::filesystem::create_directories(spritesDir);
  }

  bool allSaved = true;
  for (const auto& [spriteId, texture] : sprites) {
      std::string spritePath = spritesDir + std::to_string(spriteId) + ".png";
      sf::Image image = texture.copyToImage();
      if (!image.saveToFile(spritePath)) {
          allSaved = false;
      }
  }
  return allSaved;
}

std::map<uint, sf::Texture> ProjectsManager::loadSprites(uint projectId) {
    std::map<uint, sf::Texture> textures;
    std::string spritesDir = getProjectPath(projectId).toStdString() + "/sprites/";

    if (!std::filesystem::exists(spritesDir)) return textures;

    for (const auto &entry : std::filesystem::directory_iterator(spritesDir)) {
        if (!entry.is_regular_file() || entry.path().extension() != ".png") continue; //bérifie que c'est un png

        try {
            uint spriteId = std::stoul(entry.path().stem().string()); //transfome le nom du fichier en uint
            sf::Texture texture;
            if (texture.loadFromFile(entry.path().string())) {
                textures[spriteId] = std::move(texture); 
            } else {
                qWarning() << "Impossible de charger le sprite :" << QString::fromStdString(entry.path().string());
            }
        } catch (...) {
            qWarning() << "Fichier ignoré (ID invalide) :" << QString::fromStdString(entry.path().string());
        }
    }
    return textures;
}