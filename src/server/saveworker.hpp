#ifndef SAVEWORKER_HPP
#define SAVEWORKER_HPP

#include <atomic>
#include "mutexqueue.hpp"
#include "savetask.hpp"
#include "datamanager/projectsmanager.hpp"

class SaveWorker {
public:
    explicit SaveWorker(MutexQueue<SaveTask>& saveQueue);
    ~SaveWorker() = default;
    
    void run();
    void stop();

private:
    std::atomic<bool> mRunning_;
    MutexQueue<SaveTask>& saveQueue_;
    
    ProjectsManager projManagerLocal_; 
};

#endif