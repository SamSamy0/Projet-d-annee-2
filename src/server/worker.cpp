#include "worker.hpp"
#include <iostream>
#include <random>

Worker::Worker(MutexQueue<IMessage>& demQueue, MutexQueue<Reponse>& repQueue, MutexQueue<SaveTask>& saveQueue) 
    : demQueue_(demQueue), repQueue_(repQueue), mRunning_(true), saveQueue_(saveQueue) {}

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
  saveQueue_.stop();
}

void Worker::pushNetwork(std::unique_ptr<Reponse> rps) {
  repQueue_.push(std::move(rps));
}

void Worker::pushSave(std::unique_ptr<SaveTask> save) {
    saveQueue_.push(std::move(save));
}


uint Worker::verifyLogin(const std::string& pseudo, const std::string& password) {
    return dbManager_.verifyLogin(pseudo, password);
}

uint Worker::addUser(const std::string &pseudo, const std::string &password) {
  return dbManager_.addUser(pseudo, password);
}

bool Worker::addLink(const uint userId, const uint projectId,
                     const uint8_t role) {
  return dbManager_.addLink(userId, projectId, role);
}

bool Worker::changeRole(const uint userId, const uint projectId,
                        const uint8_t role) {
  return dbManager_.changeRole(userId, projectId, role);
}

uint Worker::addProjectSQL(const std::string &name, const uint userId) {
  return dbManager_.addProject(name, userId);
}

std::vector<MemberEntry> Worker::getProjectMembers(const uint projectId) {
  return dbManager_.getProjectMembers(projectId);
}

std::vector<ProjectEntry> Worker::getUserProjects(const uint userId) {
  return dbManager_.getUserProjects(userId);
}

uint8_t Worker::getRole(const uint userId, const uint projectId) {
  return dbManager_.getRole(userId, projectId);
}

std::vector<ProjectEntry> Worker::getAllProjects() {
  return dbManager_.getAllProjects();
}

bool Worker::renameProject(uint projectId, const std::string &newName) {
  bool dbRename = dbManager_.updateProjectName(projectId, newName);
  bool jsonRename = projManager_.updateProjectName(
      projectId, QString::fromStdString(newName));
  return dbRename && jsonRename;
}

std::string Worker::generateShareToken(uint8_t role, uint projectId) {
  std::string alphanum =
      "abcdefghijklmnopqrstuvwxyzABCDEFJHIJKLMNOPQRSTUVWXYZ123456789";
  std::random_device rd;
  std::mt19937 generator(rd());

  // Create a distribution to uniformly select from all
  // characters
  std::uniform_int_distribution<> distribution(0, alphanum.size() - 1);
  std::string token = "";
  int length = 8;
  for (int i = 0; i <= length; i++) {
    token += alphanum[distribution(generator)];
  }

  if (dbManager_.saveToken(token, role, projectId, 0)) {
    return token;
  };
  return "FFFFF";
}

bool Worker::checkShareToken(uint userId, std::string token) {
  bool isCorrect = dbManager_.checkToken(userId, token);
  return isCorrect;
}

uint Worker::duplicateProject(uint oldId, const std::string &newName,
                              uint userId) {
  uint newId = dbManager_.dupProj(newName, userId);
  // Duplication didn't work
  if (newId == -1)
    return -1;

  bool cpyFldr = projManager_.copyProjectFolder(oldId, newId);
  bool updtJson =
      projManager_.updateJsonDup(newId, QString::fromStdString(newName));
  // Copy of project folder didn't work
  if (!cpyFldr && updtJson)
    return -2;
  return newId;
}
    
QJsonObject Worker::loadProjectJson(uint projectId) {
  return projManager_.loadProjectJson(projectId);
}

bool Worker::deleteProject(uint projectId) {
  if (dbManager_.removeProject(projectId) and
      projManager_.deleteProject(projectId)) {
    return true;
  }
  return false;
}
bool Worker::removeLink(const uint userId, const uint projectId) {
  return dbManager_.removeLink(userId, projectId);
}

bool Worker::writeProjetJson(QJsonObject& jsonObject, uint id) {
    return projManager_.writeProjetJson(jsonObject, id);
}
