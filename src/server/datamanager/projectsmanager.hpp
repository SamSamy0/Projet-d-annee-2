#ifndef PROJECTFILESYSTEM_H
#define PROJECTFILESYSTEM_H

#include <QDir>
#include <QJsonObject>
#include <QString>

class ProjectsManager {
public:
  explicit ProjectsManager(const std::string &rootPath = "BigData");
  bool createProjectJson(int id, const QString &projectName, int width,
                         int height, uint scale);
  QJsonObject loadProjectJson(int id);
  bool saveImage(int id, const QString &fileName, const QByteArray &data);
  bool updateProjectName(int id, const QString &newName);
  bool copyProjectFolder(int oldId, int newId);
  bool copyRecursively(const QString &srcPath, const QString& destinationPath);
  bool updateJsonDup(int newId, const QString& newName);
  bool deleteProject(int id);
  void addCalque(int projetId, int largeur, int hauteur, int calqueId);

private:
  QString getProjectPath(int id) const;
  bool ensureDirectoryExists(int id) const;
  QString rootPath_;
};

#endif
