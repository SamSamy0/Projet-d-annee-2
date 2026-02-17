#include "reponsequeue.hpp"

void ReponseQueue::push(std::unique_ptr<Reponse> rps) {
    {
        std::lock_guard<std::mutex> lock(r_mutex);
        r_queue.push_back(std::move(rps));
    }
    r_cv.notify_one();
}

std::unique_ptr<Reponse> ReponseQueue::pop() {
    std::unique_lock<std::mutex> lock(r_mutex);
    r_cv.wait(lock, [this] { 
        return !r_queue.empty() || r_stopping; 
    });

    if (r_stopping && r_queue.empty()) return nullptr;

    auto rps = std::move(r_queue.front());
    r_queue.pop_front();
    return rps;
}

bool ReponseQueue::isEmpty() {
    std::lock_guard<std::mutex> lock(r_mutex);
    return r_queue.empty();
}

void ReponseQueue::stop() {
    {
        std::lock_guard<std::mutex> lock(r_mutex);
        r_stopping = true;
    }
    r_cv.notify_all();
}