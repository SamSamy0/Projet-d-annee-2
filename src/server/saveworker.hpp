#ifndef SAVEWORKER_HPP
#define SAVEWORKER_HPP

#include <atomic>
#include "mutexqueue.hpp"
#include "savetask.hpp"
#include "datamanager/projectsmanager.hpp"
#include "reponse/reponse.hpp"

class SaveWorker {
public:
    explicit SaveWorker(MutexQueue<SaveTask>& saveQueue, MutexQueue<Reponse>& repQueue);
    ~SaveWorker() = default;
    
    void run();
    void stop();
    void addReponse(unique_ptr<Reponse> reponse);
    ProjectsManager& getPrjMngr();

private:
    std::atomic<bool> mRunning_;
    MutexQueue<SaveTask>& saveQueue_;
    MutexQueue<Reponse>& repQueue_;
    ProjectsManager projManagerLocal_; 
};

#endif