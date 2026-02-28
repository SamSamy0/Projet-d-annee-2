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
        std::cout << "nouvelle machine connécté: "<< std::endl ;
        std::cout << client_list_.size() << std::endl;

        return true;
    } else return false;
};


//recois les messages de tout les clients
void ServerNetworkManager::getMessages(){
    //std::cout<<"getMessage : Entrée"<<std::endl;
    for (auto client : client_list_){
        //std::cout<<"getMessage : Dans la boucle"<<std::endl;
        auto packet = std::make_shared<sf::Packet>();
        
        if (client->sock->receive(*packet) == sf::Socket::Status::Done){
            std::cout<<"getMessage : Dans le if, message recu"<<std::endl;
            auto msg = MessageFactory(packet,client);
            message_queu_.push(std::move(msg));
        }
    };
};


//envoi des reponses au client
void ServerNetworkManager::sendReponse(){
    std::cout << "sendreponse:: Entrée" << std::endl;

    while (!rep_queu_.isEmpty()){
        std::cout << "sendreponse:: pop1 avant" << std::endl;
        auto rep = rep_queu_.pop();
        if (rep->message_type == MsgProtocole::AUTH_RESULT) {
            std::cout << "prob de pointeur" << std::endl;
        }
        std::cout << "sendreponse:: pop1 après" << std::endl;
            //envoi a tout les clients de la liste d'id
        for (auto id : rep->id_list){
            std::cout << "rep Prot id " << static_cast<uint8_t>(rep->message_type) << std::endl; 
            
            for (auto& client: client_list_){
                //std::cout << client->id << "  cliented ???" <<std::endl;
                //std::cout << id << std::endl;
                if (id == client->id){
                    std::cout << "envoyed ??" << std::endl;
                    client->sock->send(*(rep->packet));
                }
            }
        }
    }
    std::cout << "sendreponse:: sortie" << std::endl;
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




