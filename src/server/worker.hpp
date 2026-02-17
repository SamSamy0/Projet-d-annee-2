#ifndef WORKER_HPP
#define WORKER_HPP

#include <atomic>
#include "messagequeue.hpp"
#include "reponsequeue.hpp"
#include "databasemanager.hpp"
#include "projectsmanager.hpp"

class Worker {
public:
    explicit Worker(MessageQueue& d_queue, ReponseQueue& r_queue);
    ~Worker() = default;
    void run();
    void stop();
    long long verifyLogin(const std::string& pseudo, const std::string& password);
    long long addUser(const std::string& pseudo, const std::string& password);
    MessageQueue& dem_queue;
    ReponseQueue& rep_queue;
    DatabaseManager db_Manager;
    ProjectsManager proj_Manager;

private:
    std::atomic<bool> m_running;
};

#endif