#include "worker.hpp"
#include <iostream>

Worker::Worker(MutexQueue<IMessage>& demQueue, MutexQueue<Reponse>& repQueue) 
    : demQueue_(demQueue), repQueue_(repQueue), mRunning_(true) {}

void Worker::run() {
    std::cout << "[Worker] Démarré et en attente de messages..." << std::endl;
    std::unique_ptr<IMessage> request;
    while (mRunning_) {
        request = demQueue_.pop();

        if (request) {
            request->process(*this);
        } else {
            break;
        }
    }
}


void Worker::stop() {
    mRunning_ = false;
    repQueue_.stop(); 
    demQueue_.stop();
}


void Worker::pushNetwork(std::unique_ptr<Reponse> rps) {
    repQueue_.push(std::move(rps));
}


long long Worker::verifyLogin(const std::string& pseudo, const std::string& password) {
    return dbManager_.verifyLogin(pseudo, password);
}

long long Worker::addUser(const std::string& pseudo, const std::string& password) {
    return dbManager_.addUser(pseudo, password);
}

bool Worker::addLink(const long long userId, const long long projectId, const int8_t role) {
    return dbManager_.addLink(userId, projectId, role);
}

bool Worker::changeRole(const long long userId, const long long projectId, const int8_t role) {
    return dbManager_.changeRole(userId, projectId, role);
}

long long Worker::addProjectSQL(const std::string& name, const long long userId) {
    return dbManager_.addProject(name, userId);
}

std::vector<MemberEntry> Worker::getProjectMembers(const long long projectId) {
    return dbManager_.getProjectMembers(projectId);
}

std::vector<ProjectEntry> Worker::getUserProjects(const long long userId) {
    return dbManager_.getUserProjects(userId);
}
    
int8_t Worker::getRole(const long long userId, const long long projectId) {
    return dbManager_.getRole(userId, projectId);
}

std::vector<ProjectEntry> Worker::getAllProjects() {
    return dbManager_.getAllProjects();
}

bool Worker::renameProject(int projectId, const std::string& newName){
    bool dbRename = dbManager_.updateProjectName(projectId, newName);
    bool jsonRename = projManager_.updateProjectName(projectId, QString::fromStdString(newName));
    return dbRename && jsonRename;
}

bool Worker::duplicateProject(int oldId, const std::string& newName, long long userId){
    long long newId = dbManager_.dupProj(newName, userId);
    // Duplication didn't work
    if (newId == -1) return false;
    bool cpyFldr = projManager_.copyProjectFolder(oldId, newId);
    bool updtJson = projManager_.updateJsonDup(newId, QString::fromStdString(newName));
    return cpyFldr && updtJson;
}


bool Worker::createProjectJson(int projectId, const std::string &projectName, int width, int height, uint scale) {
    return projManager_.createProjectJson(projectId, QString::fromStdString(projectName), width, height, scale);
}
    
QJsonObject Worker::loadProjectJson(int projectId) {
    return projManager_.loadProjectJson(projectId);
}

bool Worker::saveImage(int projectId, const std::string &fileName, const QByteArray &data) {
    return projManager_.saveImage(projectId, QString::fromStdString(fileName), data);
}

bool Worker::deleteProject(int projectId) {
    if (dbManager_.removeProject(projectId) and projManager_.deleteProject(projectId)) {
        return true;
    }
    return false;
}

QByteArray Worker::getByteJson(int projectId) {
    return projManager_.getByteJson(projectId);
}