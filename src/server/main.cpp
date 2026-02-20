#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include "messages.hpp"
#include "messagequeue.hpp"
#include "servernetwork.hpp"
#include "worker.hpp"

int main() {
    MessageQueue m_queue;
    ReponseQueue r_queue;

    ServerNetworkManager servManager(m_queue, r_queue);
    Worker worker(m_queue, r_queue);

    // 1. On lance le RÉSEAU dans un thread séparé
    std::thread networkThread(&ServerNetworkManager::run, &servManager);
    std::cout << "[Main] Réseau lancé dans un thread." << std::endl;

    // 2. Le Main devient le Worker (bloquant ici)
    worker.run(); 

    // 3. Nettoyage à la fin
    if (networkThread.joinable()) {
        networkThread.join();
    }

    return 0;
}
