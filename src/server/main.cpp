#include "servernetwork.hpp"
#include "worker.hpp"
#include <thread>
#include <QCoreApplication>
#include "saveworker.hpp"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    MutexQueue<IMessage> mesQ;
    MutexQueue<Reponse> repQ;
    MutexQueue<SaveTask> saveQ;
    ServerNetworkManager servManager(mesQ, repQ);
    Worker worker(mesQ, repQ, saveQ);
    SaveWorker saveWorker(saveQ);

    if (!servManager.start()) {
        return 0;
    }

    std::thread networkThread(&ServerNetworkManager::run, &servManager);
    std::cout << "[Main] Réseau lancé dans un thread." << std::endl;

    std::thread saveThread(&SaveWorker::run, &saveWorker);
    std::cout << "[Main] Saveur lancé dans un thread." << std::endl;

    worker.run(); 

    if (networkThread.joinable()) {
        networkThread.join();
    }

    if (saveThread.joinable()) {
        saveThread.join();
    }

}
