#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <vector>
#include "nlohmann/json.hpp"
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

namespace fs = std::filesystem;
using json = nlohmann::json;

const std::string DATA_DIR = "serveur_data";
const std::string USER_DIR = DATA_DIR + "/users";
const std::string CONFIG_FILE = DATA_DIR + "/config.json";

void handle_client(int client_socket) {

    char buffer[1024] = {0};
    while (true) {
        int valread = read(client_socket, buffer, 1024);
        if (valread <= 0) {
            std::cout << "Un client s'est déconnecté." << std::endl;
            break;
        }
        std::cout << "Message reçu : " << buffer << " id " << client_socket <<std::endl;

        send(client_socket, buffer, valread, 0);
        memset(buffer, 0, 1024);
    }
    close(client_socket);
}

void setup_environment() {

    std::cout << "[INIT] Verification de l'environnement..." << std::endl;
    std::vector<std::string> dirs = { DATA_DIR, USER_DIR };
    for (const std::string& d : dirs) {
        if (!fs::exists(d)) {
            fs::create_directories(d);
            std::cout << "[+] Dossier cree : " << d << std::endl;
        }
    }

    if (!fs::exists(CONFIG_FILE)) {
        json default_config;
        default_config["server_port"] = 8080;
        default_config["max_users"] = 10;

        std::ofstream ofs(CONFIG_FILE);
        ofs << default_config.dump(4);
        std::cout << "[+] Fichier config.json cree par defaut." << std::endl;
    }
}

void register_new_user_test(const std::string& name, const std::string& password) {
    std::string path = USER_DIR + "/" + name;
    if (fs::exists(path)) {
        std::cout << "[!] L'utilisateur " << name << " existe deja." << std::endl;
        return;
    }

    fs::create_directories(path + "/projects");

    json auth;
    auth["username"] = name;
    auth["password"] = password; //encoer  un jour lol

    std::ofstream ofs(path + "/auth.json");
    ofs << auth.dump(4);
    std::cout << "[OK] Utilisateur " << name << " enregistre." << std::endl;
}

int main() {
    setup_environment();

    std::cout << "--- Serveur GIMP-Share Pret ---" << std::endl;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 10); //le max c 10

    std::cout << "Serveur multi-client prêt sur le port 8080..." << std::endl;

    while (true) {
        int addrlen = sizeof(address);
        int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        if (new_socket >= 0) {
            std::cout << "Nouveau client connecté !" << std::endl;

            std::thread(handle_client, new_socket).detach();
        }
    }

    close(server_fd);
    return 0;
}