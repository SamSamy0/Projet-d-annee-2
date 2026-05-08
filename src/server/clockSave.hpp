#pragma once
#include "message/message.hpp"
#include "mutexqueue.hpp"
#include <mutex>
#include <condition_variable>
#include <atomic>

class ClockSave {
    MutexQueue<IMessage> &messageQueue_;
    atomic<bool> running_ = true;
    std::mutex mutex_;
    std::condition_variable condition_;
public:
    ClockSave(MutexQueue<IMessage> &queue) : messageQueue_(queue) {}

    void run();
    void stop();
};