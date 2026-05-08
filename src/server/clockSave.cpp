#include "clockSave.hpp"

void ClockSave::run() {
    while (running_) {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            bool arret_demande = condition_.wait_for(lock, std::chrono::seconds(30), [this]() {
                return !running_;
            });

            if (arret_demande) {
                break; 
            }
        }
        std::unique_ptr<IMessage> saveMsg = std::make_unique<SaveAllMessage>();
        messageQueue_.push(std::move(saveMsg));
    }
}


void ClockSave::stop() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        running_ = false;
    }
    condition_.notify_all(); 
    std::cout << "[ClockSave] Arrêt demandé." << std::endl;
}