#include "servernetwork.hpp"
#include "worker.hpp"
#include <thread>
#include <QCoreApplication>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    MutexQueue<IMessage> mesQ;
    MutexQueue<Reponse> repQ;
    ServerNetworkManager servManager(mesQ, repQ);
    Worker worker(mesQ, repQ);
    
    std::thread networkThread(&ServerNetworkManager::run, &servManager);
    std::cout << "[Main] Réseau lancé dans un thread." << std::endl;

    worker.run(); 

    if (networkThread.joinable()) {
        networkThread.join();
    }

}