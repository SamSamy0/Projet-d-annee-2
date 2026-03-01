#include "servernetwork.hpp"
#include <iostream>
#include <SFML/Network.hpp>



ServerNetworkManager::ServerNetworkManager(MutexQueue<IMessage>& mes , MutexQueue<Reponse>& rep)
: messageQueu_(mes), repQueu_(rep){}


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

        client_list.push_back(std::move(new_client));
        std::cout << "nouvelle machine connécté: "<< std::endl ;
        std::cout << client_list.size() << std::endl;

        return true;
    } else return false;
};


//recois les messages de tout les clients
void ServerNetworkManager::getMessages(){
    //std::cout<<"getMessage : Entrée"<<std::endl;
    for (auto client : client_list){
        //std::cout<<"getMessage : Dans la boucle"<<std::endl;
        auto packet = sf::Packet();
        
        if (client->sock->receive(packet) == sf::Socket::Status::Done){
            std::cout<<"getMessage : Dans le if, message recu"<<std::endl;
            auto msg = MessageFactory(packet, client);
            messageQueu_.push(std::move(msg));
        }
    };
};


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
    mRunning_ = true;
    start();
    std::cout << "[Network] Serveur démarré, prêt à gérer les clients..." << std::endl;

    while (mRunning_) {
        accept();
        getMessages();
        sendReponse();

        //pour faire une vérif des messages toutes 10 millisecs
        sf::sleep(sf::milliseconds(1000)); 
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



