#include "worker.hpp"
#include <iostream>

Worker::Worker(MutexQueue<IMessage>& demQueue, MutexQueue<IMessage>& repQueue) 
    : demQueue_(demQueue), repQueue_(repQueue), mRunning_(true) {}

void Worker::run() {
    std::cout << "[Worker] Démarré et en attente de messages..." << std::endl;
    
    while (mRunning_) {
    std::unique_ptr<IMessage> request = demQueue_.pop();
    std::cout << "[Worker] pop()..." << std::endl;

        if (request) {
            std::cout << "[Worker] Traitement d'un nouveau message..." << std::endl;
            request->process(*this);
        }/* else {
            break;
        }*/
    }
}


void Worker::stop() {
    mRunning_ = false;
    repQueue_.stop(); 
    demQueue_.stop();
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

bool Worker::createProjectJson(int projectId, const std::string &projectName, int width, int height, uint scale) {
    return projManager_.createProjectJson(projectId, QString::fromStdString(projectName), width, height, scale);
}
    
QJsonObject Worker::loadProjectJson(int projectId) {
    return projManager_.loadProjectJson(projectId);
}

bool Worker::saveImage(int projectId, const std::string &fileName, const QByteArray &data) {
    return projManager_.saveImage(projectId, QString::fromStdString(fileName), data);
}