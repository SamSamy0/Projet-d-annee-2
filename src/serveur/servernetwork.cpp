#include "servernetwork.hpp"
#include <iostream>
#include <SFML/Network.hpp>
#include <algorithm>



ServerNetworkManager::ServerNetworkManager(MutexQueue<IMessage>& mes , MutexQueue<Reponse>& rep)
: messageQueu_(mes), repQueu_(rep){}


bool ServerNetworkManager::start() {
    if (listener_.listen(5000) != sf::Socket::Status::Done) {
        std::cout << "Erreur : Impossible d'écouter sur le port "<< std::endl;
        return false;
    }
    selector_.add(listener_);

    std::cout << "Serveur lance avec succes sur le port " << std::endl;
    std::cout << "En attente de connexions..." << std::endl;

    mRunning_ = true;
    return true;
}


void ServerNetworkManager::listen() {
    while(mRunning_) {
        if (selector_.wait()) {
            if (selector_.isReady(listener_)) {
                handleNewConnection();
            }
            
            handleClientMessages();
        }
        std::cout << "finito" << std::endl;
    }
}

void ServerNetworkManager::handleNewConnection() {
    auto socket_client = std::make_shared<sf::TcpSocket>();
    
    if (listener_.accept(*socket_client) == sf::Socket::Status::Done) {
        socket_client->setBlocking(false);

        auto new_client = std::make_shared<Client>(); 
        new_client->sock = socket_client;

        selector_.add(*socket_client);

        client_list.push_back(std::move(new_client));
        std::cout << "nouvelle machine connécté: "<< std::endl ;
        std::cout << client_list.size() << std::endl;
    }
}


void ServerNetworkManager::handleClientMessages() {
    auto it = std::remove_if(client_list.begin(), client_list.end(), [this](auto client) {
        sf::Packet packet;
        auto status = client->sock->receive(packet);

        if (status == sf::Socket::Status::Done) {
            messageQueu_.push(MessageFactory(packet, client));
            return false;
        }
        
        if (status == sf::Socket::Status::Disconnected || status == sf::Socket::Status::Error) {
            selector_.remove(*client->sock);
            return true;
        }
        return false;
    });

    client_list.erase(it, client_list.end());
}



//envoi des reponses au client
void ServerNetworkManager::sendReponse(){
    std::cout << "sendreponse:: Entrée" << std::endl;

    while (!repQueu_.isEmpty()){
        std::cout << "sendreponse:: pop1 avant" << std::endl;
        auto rep = repQueu_.pop();
        
    }
    std::cout << "sendreponse:: sortie" << std::endl;
};


void ServerNetworkManager::run() {
    if (!start()) return;

    listenThread_ = std::thread(&ServerNetworkManager::listen, this);

    std::cout << "Serveur actif. Tapez 'exit' pour arreter." << std::endl;

    while (mRunning_) {
        std::string commande;
        std::cin >> commande; 
        if (commande == "exit") {
            mRunning_ = false; // Arrête la boucle du thread réseau aussi !
        }
    }

    stop(); 
}

void ServerNetworkManager::stop() {
    mRunning_ = false; 
    
    if (listenThread_.joinable()) {
        listenThread_.join();
    }
}

void debugAuthPacket(sf::Packet copy) {
    uint8_t type;
    uint8_t pseudo;
    std::cout << copy.getDataSize() << std::endl;

    copy >> type;
    std::cout << "type de messqages:"<< static_cast<int>(type) << std::endl;

    copy >> pseudo;
     std::cout << "int reussi:"<< static_cast<int>(pseudo)<< std::endl;

    
}



