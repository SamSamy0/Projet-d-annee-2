#ifndef PROJECTFILESYSTEM_H
#define PROJECTFILESYSTEM_H

#include <QDir>
#include <QJsonObject>
#include <QString>

class ProjectsManager {
public:
  explicit ProjectsManager(const std::string &rootPath = "BigData");
  bool createProjectJson(uint id, const QString &projectName, uint width,
                         uint height, uint scale);
  QJsonObject loadProjectJson(uint id);
  bool saveImage(uint id, const QString &fileName, const QByteArray &data);
  bool updateProjectName(uint id, const QString &newName);
  bool copyProjectFolder(uint oldId, uint newId);
  bool copyRecursively(const QString &srcPath, const QString& destinationPath);
  bool updateJsonDup(uint newId, const QString& newName);
  bool deleteProject(uint id);
  void addCalque(uint projetId, uint largeur, uint hauteur, uint calqueId);
  QByteArray getByteJson(uint projetId);
  QByteArray getByteImages(uint projetId);
  bool writeProjetJson(QJsonObject& jsonObject, uint id);

private:
  QString getProjectPath(uint id) const;
  bool ensureDirectoryExists(uint id) const;
  QString rootPath_;
};

#endif
