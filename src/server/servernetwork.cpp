#include <SFML/Network.hpp>
#include <vector>
#include <iostream>
#include <array>
#include <memory>
#include <deque>
#include "protocol.hpp"





struct Client{
    int id = 0;
    std::shared_ptr<sf::TcpSocket> sock;
    sf::IpAddress getAddress(){
        return sock->getRemoteAddress();
    }
};


struct Event{
    Client client;
    MsgProtocole message_type;
    std::shared_ptr<sf::Packet> data_packet;

};

struct Reponse{
    Client client;
    std::vector<int> id_list;
    MsgProtocole message_type;
    std::unique_ptr<sf::Packet> packet;
};


class ServerNetworkManager{
    sf::TcpListener listener;
    std::vector<Client> client_list;
    std::deque<Event> event_queu;
    std::deque<Reponse> rep_queu;
    public:

        // lance le serveur en ecoute
        bool start(){
            if (listener.listen(5000) == sf::Socket::Done){  
                std::cout << "Le server écoute sur le port 5000" << std::endl;
                listener.setBlocking(false);  // met le listener en non-bloquant
                return true;
            }else return false;    
        };


        //accepte les clients qui se connectes
        bool accept(){

            //crée un pointeur vers le socket client
            auto socket_client = std::make_shared<sf::TcpSocket>();

            
            if (listener.accept(*socket_client) == sf::Socket::Done) {

                socket_client->setBlocking(false);

                Client new_client;
                new_client.sock = socket_client;

                client_list.push_back(new_client);
                std::cout << "nouvelle machine connécté: " << socket_client->getRemoteAddress() << std::endl ;

                return true;
            } else return false;
        };


        //recois les messages de tout les clients
        void getMessages(){
            for (auto& client : client_list){

                Event recu;
                recu.client = client;
                auto packet = std::make_shared<sf::Packet>();
                uint8_t msg_type;
                
                if (client.sock->receive(*packet) == sf::Socket::Done){
                    *packet >> msg_type;
                    recu.message_type = static_cast<MsgProtocole>(msg_type) ;
                    recu.data_packet = std::move(packet);
                    event_queu.push_back(recu);
                    
                    std::cout << " LOGS!!! PAS ENCORE COMFIGURER" << std::endl;
                }
            };
        };


        //envoi des reponses au client
        void sendReponse(){
            for (auto it = rep_queu.begin(); it != rep_queu.end(); ){
                auto& rep = *it;

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
                
            it = rep_queu.erase(it);
            };
        };

};



