#ifndef PROJECTFILESYSTEM_H
#define PROJECTFILESYSTEM_H

#include <QDir>
#include <QJsonObject>
#include <QString>
#include "../sprite.hpp"
#include <QFile>
#include <SFML/Graphics/Texture.hpp>
#include <map>

class ProjectsManager {
public:
  explicit ProjectsManager(const std::string &rootPath = "projectsFolder");
  QJsonObject loadProjectJson(uint id);
  bool saveImage(uint id, uint imageId, const QImage &data);
  QImage loadImage(uint projectId, uint layerId);
  bool updateProjectName(uint id, const QString &newName);
  bool copyProjectFolder(uint oldId, uint newId);
  bool copyRecursively(const QString &srcPath, const QString& destinationPath);
  bool updateJsonDup(uint newId, const QString& newName);
  bool deleteProject(uint id);
  bool writeProjetJson(QJsonObject& jsonObject, uint id);
  bool saveSpriteLayer(uint id, uint imageId, QJsonObject& SpriteLayer);
  QJsonObject loadSpriteLayer(uint id, uint layerId);
  bool saveChat(uint projectId, QJsonArray& chat);
  QJsonArray loadChat(uint projectId);

  bool saveSprite(uint projectId, std::map<uint, sf::Texture> &sprites);
  std::map<uint, sf::Texture> loadSprites(uint projectId);

  QByteArray Zip(uint projectId);

private:
  QString getProjectPath(uint id) const;
  bool ensureDirectoryExists(uint id) const;
  QString rootPath_;
};

#endif
