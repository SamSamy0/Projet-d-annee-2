#include "worker.hpp"
#include <thread>



void clientSim(const std::string& nom, const std::string& passW, const std::string& Proj, MutexQueue<IMessage>& demQueue) {
    sf::Packet packet;
    packet << nom << passW;
    std::unique_ptr<IMessage> msg1, msg2, msg3, msg4, msg5;
    msg1 = std::make_unique<LoginMessage>(packet);
    demQueue.push(std::move(msg1));

    packet.clear();
    packet << nom << passW;
    msg2 = std::make_unique<LoginMessage>(packet);
    demQueue.push(std::move(msg2));

    packet.clear();
    sf::Vector2u taille;
    taille.x = 1200;
    taille.y = 800;
    float scale = 50;
    packet << Proj << taille.x << taille.y << 50;
    msg3 = std::make_unique<CreateProjectMessage>(packet, 1);
    demQueue.push(std::move(msg3));

    packet.clear();
    msg4 = std::make_unique<GetUsersProjectsMessage>(packet, 1);
    demQueue.push(std::move(msg4));
}


int main() {
    MutexQueue<IMessage> repQueue;
    MutexQueue<IMessage> demQueue;
    Worker taff(demQueue, repQueue);

    std::thread clientThread(&clientSim, "Chinzo", "1234556", "jesuisunprojet", std::ref(demQueue));

    taff.run();

}