#include "servernetwork.hpp"
#include "worker.hpp"
#include <thread>

int main() {
    MutexQueue<IMessage> mesQ;
    MutexQueue<Reponse> repQ;
    ServerNetworkManager servManager(mesQ, repQ);
    Worker worker(mesQ, repQ);
    
    std::thread networkThread(&ServerNetworkManager::run, &servManager);
    std::cout << "[Main] Réseau lancé dans un thread." << std::endl;

    // 2. Le Main devient le Worker (bloquant ici)
    worker.run(); 

    // 3. Nettoyage à la fin
    if (networkThread.joinable()) {
        networkThread.join();
    }

}