#include "servernetwork.hpp"
#include <iostream>
#include <SFML/Network.hpp>


ServerNetworkManager::ServerNetworkManager(MessageQueue& mes , ReponseQueue& rep)
: message_queu_(mes), rep_queu_(rep){}


bool ServerNetworkManager::start(){
    if (listener_.listen(5000) == sf::Socket::Status::Done){  
        std::cout << "Le server écoute sur le port 5000" << std::endl;
        listener_.setBlocking(false);  // met le listener en non-bloquant
        return true;
    }else return false;    
};


//accepte les clients qui se connectes
bool ServerNetworkManager::accept(){
    auto socket_client = std::make_shared<sf::TcpSocket>();

    if (listener_.accept(*socket_client) == sf::Socket::Status::Done) {
        socket_client->setBlocking(false);

        auto new_client = std::make_shared<Client>(); 
        new_client->sock = socket_client;

        client_list_.push_back(std::move(new_client));
        std::cout << "nouvelle machine connécté "<< std::endl ;

        return true;
    } else return false;
};


//recois les messages de tout les clients
void ServerNetworkManager::getMessages(){
    for (auto client : client_list_){
        auto packet = std::make_shared<sf::Packet>();
        
        if (client->sock->receive(*packet) == sf::Socket::Status::Done){
            auto msg = MessageFactory(packet,client);
            message_queu_.push(std::move(msg));
        }
    };
};


//envoi des reponses au client
void ServerNetworkManager::sendReponse(){
    while (!rep_queu_.isEmpty()){
        auto rep = rep_queu_.pop();
        //envoi a tout les clients de la liste d'id
        for (auto id : rep->id_list){
            for (auto& client: client_list_){
                if (id == client->id){
                    std::cout << "[LOG] " << to_string(rep->message_type) << ": au client " << id << std::endl;
                    client->sock->send(*(rep->packet));
                }
            }
        }
    }
};


void ServerNetworkManager::run() {
    m_running_ = true;
    start();
    std::cout << "[Network] Serveur démarré, prêt à gérer les clients..." << std::endl;

    while (m_running_) {
        accept();
        getMessages();
        sendReponse();

        //pour faire une vérif des messages toutes 10 millisecs
        sf::sleep(sf::milliseconds(1000)); 
    }
}




