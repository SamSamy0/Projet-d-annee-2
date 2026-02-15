#ifndef PROJECTFILESYSTEM_H
#define PROJECTFILESYSTEM_H

#include <QString>
#include <QJsonObject>
#include <QDir>

class ProjectsManager {
public:

    explicit ProjectsManager(const std::string &rootPath = "BigData");
    bool saveProjectJson(int id, const QJsonObject &data);
    bool saveImage(int id, const QString &fileName, const QByteArray &data);

private:
    QString getProjectPath(int id) const;
    bool ensureDirectoryExists(int id) const;
    QString m_rootPath;
};

#endif
