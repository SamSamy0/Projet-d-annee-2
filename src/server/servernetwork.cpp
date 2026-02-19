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

        Client new_client;
        new_client.sock = socket_client;

        client_list.push_back(new_client);
        std::cout << "nouvelle machine connécté: "<< std::endl ;

        return true;
    } else return false;
};


//recois les messages de tout les clients
void ServerNetworkManager::getMessages(){
    for (auto client : client_list){

        auto packet = std::make_shared<sf::Packet>();
        
        if (client.sock->receive(*packet) == sf::Socket::Status::Done){
            auto msg = MessageFactory(packet,client);
            message_queu.push(std::move(msg));
        }
    };
};


//envoi des reponses au client
void ServerNetworkManager::sendReponse(){
    while (!rep_queu.isEmpty()){
        Reponse& rep = *rep_queu.pop();

        //si c'est pour une connection
        if (rep.message_type == MsgProtocole::AUTH_RESULT){

            //associe au socket l'id du client
            for (auto& client: client_list){
                if (rep.client.getAddress() == client.getAddress()){
                    client.id = rep.id_list[0];
                }
            }

            //envoie la reponse serveur
            rep.client.sock->send(*(rep.packet));
        }

        //sinon
        else{
            //envoi a tout les clients de la liste d'id
            for (auto id : rep.id_list){
                for (auto& client: client_list){
                    if (id == client.id){
                        client.sock->send(*(rep.packet));
                    }
                }
            }
        }
    };
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
        sf::sleep(sf::milliseconds(10)); 
    }
}



