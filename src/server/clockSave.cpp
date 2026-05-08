#include "clockSave.hpp"

void ClockSave::run() {
    sf::Clock timer;
    while (running_) {
        sf::sleep(sf::milliseconds(500));

        // Si 30 secondes se sont écoulées
        if (timer.getElapsedTime() >= sf::seconds(30)) {
            timer.restart();
            
            if (running_) {
                std::unique_ptr<IMessage> saveMsg = std::make_unique<SaveAllMessage>();
                messageQueue_.push(std::move(saveMsg));
            }
        }
    }
}

void ClockSave::stop() {
    running_ = false;
    std::cout << "[ClockSave] Arrêt demandé." << std::endl;
}