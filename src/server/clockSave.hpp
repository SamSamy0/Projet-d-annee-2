#pragma once
#include "message/message.hpp"
#include "mutexqueue.hpp"

class ClockSave {
    MutexQueue<IMessage> &messageQueue_;
    atomic<bool> running_ = true;
public:
    ClockSave(MutexQueue<IMessage> &queue) : messageQueue_(queue) {}

    void run();
    void stop();
};