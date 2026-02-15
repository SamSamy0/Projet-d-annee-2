#ifndef WORKER_HPP
#define WORKER_HPP

#include <atomic>
#include "messagequeue.hpp"
#include "databasemanager.hpp"

class Worker {
public:
    explicit Worker(MessageQueue& d_queue, MessageQueue& r_queue);
    ~Worker() = default;
    void run();
    void stop();

private:
    MessageQueue& dem_queue;
    MessageQueue& rep_queue;
    std::atomic<bool> m_running;
};

#endif