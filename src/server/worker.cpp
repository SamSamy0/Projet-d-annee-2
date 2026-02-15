#include "worker.hpp"
#include <iostream>

Worker::Worker(MessageQueue& d_queue, MessageQueue& r_queue) 
    : dem_queue(d_queue), rep_queue(r_queue), m_running(true) {}

void Worker::run() {
    std::cout << "[Worker] Démarré et en attente de messages..." << std::endl;
    
    while (m_running) {
    std::unique_ptr<Message> request = dem_queue.pop();

        if (request) {
            std::cout << "[Worker] Traitement d'un nouveau message..." << std::endl;
            request->process(*this);
        } else {
            break; 
        }
    }
}


void Worker::stop() {
    m_running = false;
    rep_queue.stop(); 
    dem_queue.stop();
}
