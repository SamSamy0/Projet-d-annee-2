#ifndef PROJECTFILESYSTEM_H
#define PROJECTFILESYSTEM_H

#include <QDir>
#include <QJsonObject>
#include <QString>

class ProjectsManager {
public:
  explicit ProjectsManager(const std::string &rootPath = "BigData");
  QJsonObject loadProjectJson(uint id);
  bool saveImage(uint id, uint imageId, const QImage &data);
  QImage loadImage(uint projectId, uint layerId);
  bool updateProjectName(uint id, const QString &newName);
  bool copyProjectFolder(uint oldId, uint newId);
  bool copyRecursively(const QString &srcPath, const QString& destinationPath);
  bool updateJsonDup(uint newId, const QString& newName);
  bool deleteProject(uint id);
  bool writeProjetJson(QJsonObject& jsonObject, uint id);

private:
  QString getProjectPath(uint id) const;
  bool ensureDirectoryExists(uint id) const;
  QString rootPath_;
};

#endif
