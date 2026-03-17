#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <mutex>
#include <condition_variable>
#include <deque>
#include <memory>


template <typename T>
class MutexQueue {
private:
    std::deque<std::unique_ptr<T>> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_stopping = false;

public:
    MutexQueue() = default;
    ~MutexQueue() = default;

    void push(std::unique_ptr<T> msg) {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push_back(std::move(msg));
        }
        m_cv.notify_one();
    }
    
    std::unique_ptr<T> pop(){
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [this] { 
            return !m_queue.empty() || m_stopping; 
        });
        
        if (m_stopping && m_queue.empty()) return nullptr;
        
        auto msg = std::move(m_queue.front());
        m_queue.pop_front();
        return msg;
    }
    
    bool isEmpty() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }

    void stop() {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stopping = true;
        }
        m_cv.notify_all();
    }

};

#endif