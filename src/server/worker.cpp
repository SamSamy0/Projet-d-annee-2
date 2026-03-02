#include "worker.hpp"
#include <iostream>

Worker::Worker(MessageQueue& d_queue, ReponseQueue& r_queue) 
    : dem_queue(d_queue), rep_queue(r_queue), m_running(true) {}

void Worker::run() {
    
    while (m_running) {
    std::shared_ptr<Message> request = dem_queue.pop();

        if (request) {
            request->process(*this);
        } else {
            break;
        }
        sf::sleep(sf::milliseconds(1000)); 
    }
}


void Worker::stop() {
    m_running = false;
    rep_queue.stop(); 
    dem_queue.stop();
}


long long Worker::verifyLogin(const std::string& pseudo, const std::string& password) {
    return db_Manager.verifyLogin(pseudo, password);
}

long long Worker::addUser(const std::string& pseudo, const std::string& password) {
    return db_Manager.addUser(pseudo, password);
}
