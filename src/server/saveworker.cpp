#include "saveworker.hpp"
#include <iostream>
#include <fstream>

SaveWorker::SaveWorker(MutexQueue<SaveTask>& saveQueue, MutexQueue<Reponse>& repQueue) 
    : saveQueue_(saveQueue), repQueue_(repQueue), mRunning_(true) {}

void SaveWorker::run() {
    std::cout << "[SaveWorker] Démarré et en attente de sauvegardes..." << std::endl;
    std::unique_ptr<SaveTask> task;
    
    while (mRunning_) {
        task = saveQueue_.pop();
        std::cout << "tentative de sauvgarde" << std::endl;
        task->execute(*this);
    }
}

void SaveWorker::stop() {
    mRunning_ = false;
    saveQueue_.stop();
}

void SaveWorker::addReponse(unique_ptr<Reponse> reponse) {
    repQueue_.push(std::move(reponse));
}

ProjectsManager& SaveWorker::getPrjMngr() {
    return projManagerLocal_;
}