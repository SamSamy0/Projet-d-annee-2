#include "clockSave.hpp"

void ClockSave::run() {
    while (running_) {
        sf::sleep(sf::seconds(30));

        if (running_) {
            std::unique_ptr<IMessage> saveMsg = std::make_unique<SaveAllMessage>();
            messageQueue_.push(std::move(saveMsg));
        }
    }
}

void ClockSave::stop() {
    running_ = false;
}