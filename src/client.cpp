#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <chrono>
#include <thread>

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    std::string message = "Hello depuis le client !";

    sock = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "Échec de connexion" << std::endl;
        return -1;
    }

    std::cout << "Connecté au serveur." << std::endl;

    int compteur = 1;
    while (true) {
        std::string message = "Client toujours en ligne - Message n°" + std::to_string(compteur);

        if (send(sock, message.c_str(), message.length(), 0) < 0) {
            std::cout << "Erreur d'envoi (serveur déconnecté ?)" << std::endl;
            break;
        }

        std::cout << "[Envoyé] " << message << std::endl;
        compteur++;

        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}