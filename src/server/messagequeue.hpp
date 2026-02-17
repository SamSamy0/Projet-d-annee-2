#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <mutex>
#include <condition_variable>
#include <deque>
#include <memory>
#include "messages.cpp"

class MessageQueue {
private:
    std::deque<std::unique_ptr<Message>> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_stopping = false;

public:
    MessageQueue() = default;
    ~MessageQueue() = default;

    void push(std::unique_ptr<Message> msg);
    std::unique_ptr<Message> pop();
    void stop();
    bool isEmpty();
};

#endif