#include "servernetwork.hpp"
#include <iostream>
#include <SFML/Network.hpp>



ServerNetworkManager::ServerNetworkManager(MessageQueue& mes , ReponseQueue& rep)
: message_queu(mes), rep_queu(rep){}


bool ServerNetworkManager::start(){
    if (listener.listen(5000) == sf::Socket::Status::Done){  
        std::cout << "Le server écoute sur le port 5000" << std::endl;
        listener.setBlocking(false);  // met le listener en non-bloquant
        return true;
    }else return false;    
};


//accepte les clients qui se connectes
bool ServerNetworkManager::accept(){
    auto socket_client = std::make_shared<sf::TcpSocket>();

    if (listener.accept(*socket_client) == sf::Socket::Status::Done) {
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
        auto packet = std::make_shared<sf::Packet>();
        
        if (client->sock->receive(*packet) == sf::Socket::Status::Done){
            std::cout<<"getMessage : Dans le if, message recu"<<std::endl;
            auto msg = MessageFactory(packet,client);
            message_queu.push(std::move(msg));
        }
    };
};


//envoi des reponses au client
void ServerNetworkManager::sendReponse(){
    std::cout << "sendreponse:: Entrée" << std::endl;

    while (!rep_queu.isEmpty()){
        std::cout << "sendreponse:: pop1 avant" << std::endl;
        Reponse& rep = *rep_queu.pop();
        std::cout << "sendreponse:: pop1 après" << std::endl;
        
            //envoi a tout les clients de la liste d'id
        for (auto id : rep.id_list){
            
            std::cout << "listed ???" << std::endl;
            for (auto& client: client_list){
                std::cout << "cliented ???" << std::endl;
                if (id == client->id){
                    std::cout << "envoyed ??" << std::endl;
                    client->sock->send(*(rep.packet));
                }
            }
        }
    }
    std::cout << "sendreponse:: sortie" << std::endl;
};

void ServerNetworkManager::run() {
    m_running = true;
    start();
    std::cout << "[Network] Serveur démarré, prêt à gérer les clients..." << std::endl;

    while (m_running) {
        accept();
        getMessages();
        sendReponse();

        //pour faire une vérif des messages toutes 10 millisecs
        sf::sleep(sf::milliseconds(1000)); 
    }
}



