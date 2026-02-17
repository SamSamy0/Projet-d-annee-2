#include "messagequeue.hpp"

void MessageQueue::push(std::unique_ptr<Message> msg) {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push_back(std::move(msg));
    }
    m_cv.notify_one();
}

std::unique_ptr<Message> MessageQueue::pop() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cv.wait(lock, [this] { 
        return !m_queue.empty() || m_stopping; 
    });

    if (m_stopping && m_queue.empty()) return nullptr;

    auto msg = std::move(m_queue.front());
    m_queue.pop_front();
    return msg;
}

bool ReponseQueue::isEmpty() {
    std::lock_guard<std::mutex> lock(r_mutex);
    return r_queue.empty();
}

void MessageQueue::stop() {
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_stopping = true;
    }
    m_cv.notify_all();
}