#include "servernetwork.hpp"
#include "worker.hpp"
#include <thread>
#include <QCoreApplication>
#include "saveworker.hpp"
#include "clockSave.hpp"
#include <csignal>

std::atomic<bool> ctrl_c_pressed{false};

void handle_sigint(int sig) {
    std::cout << "\n[System] Ctrl+C intercepté ! Arrêt des services en cours..." << std::endl;
    ctrl_c_pressed = true;
    
}

int main(int argc, char *argv[]) {
    std::signal(SIGINT, handle_sigint);
    QCoreApplication app(argc, argv);
    MutexQueue<IMessage> mesQ;
    MutexQueue<Reponse> repQ;
    MutexQueue<SaveTask> saveQ;
    ServerNetworkManager servManager(mesQ, repQ);
    Worker worker(mesQ, repQ, saveQ);
    SaveWorker saveWorker(saveQ, repQ);
    ClockSave clockSave(mesQ);

    std::thread controlThread([&mesQ]() {
        while (!ctrl_c_pressed) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        std::unique_ptr<IMessage> shutdownMsg = std::make_unique<ShutDownMessage>();
        mesQ.push(std::move(shutdownMsg));
    });

    if (!servManager.start()) {
        ctrl_c_pressed = true;
        controlThread.join();
        return 0;
    }

    std::thread networkThread(&ServerNetworkManager::run, &servManager);
    std::cout << "[Main] Réseau lancé dans un thread." << std::endl;

    std::thread saveThread(&SaveWorker::run, &saveWorker);
    std::cout << "[Main] Saveur lancé dans un thread." << std::endl;

    std::thread clockThread(&ClockSave::run, &clockSave);
    std::cout << "[Main] ClockSave lancé dans un thread." << std::endl;

    worker.run(); 

    ctrl_c_pressed = true; //si le worker s'interrompt avant le ctrl+c
    if (controlThread.joinable()) {
        controlThread.join();
        mesQ.stop();
        repQ.stop();
        saveQ.stop();
    }

    if (networkThread.joinable()) {
        networkThread.join();
    }

    if (saveThread.joinable()) {
        saveThread.join();
    }

    clockSave.stop();

    if (clockThread.joinable()) {
        clockThread.join();
    }
    
    std::cout << "[Main] Tous les threads ont été arrêtés. Fermeture du serveur." << std::endl;
    return 0;

}
