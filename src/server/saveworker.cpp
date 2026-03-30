#include "saveworker.hpp"
#include <iostream>
#include <fstream>

SaveWorker::SaveWorker(MutexQueue<SaveTask>& saveQueue) 
    : saveQueue_(saveQueue), mRunning_(true) {}

void SaveWorker::run() {
    std::cout << "[SaveWorker] Démarré et en attente de sauvegardes..." << std::endl;
    std::unique_ptr<SaveTask> task;
    
    while (mRunning_) {
        task = saveQueue_.pop();
        std::cout << "tentative de sauvgarde" << std::endl;
        std::ofstream testFile("test_thread.txt");
        testFile << "Le thread écrit bien !";
        testFile.close();

        task->execute(this->projManagerLocal_);
    }
}

void SaveWorker::stop() {
    mRunning_ = false;
    saveQueue_.stop();
}