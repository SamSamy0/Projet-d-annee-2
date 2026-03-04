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
        if (selector_.wait(sf::seconds(5))) {
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
        selector_.add(*socket_client);
        new_client->sock = std::move(socket_client);

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
            std::cout << "bye mec o7" << std::endl;
            return true;
        }
        return false;
    });

    client_list.erase(it, client_list.end());
}



//envoi des reponses au client
void ServerNetworkManager::respond(){
    std::unique_ptr<Reponse> rps;
    while(mRunning_) {
        rps = repQueu_.pop();
        rps->envoyer(*this);
    }
};


void ServerNetworkManager::run() {
    if (!start()) return;

    listenThread_ = std::thread(&ServerNetworkManager::listen, this);

    respond();

    stop(); 
}

void ServerNetworkManager::stop() {
    mRunning_ = false; 
    
    if (listenThread_.joinable()) {
        listenThread_.join();
    }
}




