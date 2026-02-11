#include <SFML/Network.hpp>
#include <vector>
#include <iostream>
#include <array>
#include <memory>
#include <deque>
#include "protocol.hpp"





struct Client{
    int id;
    std::unique_ptr<sf::TcpSocket> sock;
};


struct Event{
    Client client;
    MsgProtocole message_type;
    std::unique_ptr<sf::Packet> data_packet;
};

struct Reponse{
    Client client;
    bool synchro;
    MsgProtocole message_type;
    std::unique_ptr<sf::Packet> packet;
};


class ServerNetworkManager{
    sf::TcpListener listener;
    std::vector<Client> client_list;
    
    std::deque<Reponse> rep_queu;
    public:



        bool start(){
            if (listener.listen(5000) == sf::Socket::Done){
                std::cout << "Le server écoute sur le port 5000" << std::endl;
                listener.setBlocking(false);
                return true;
            }else return false;    
        };

        bool accept(){

            auto socket_client = std::make_unique<sf::TcpSocket>();
            if (listener.accept(*socket_client) == sf::Socket::Done) {
                
                Client new_client;
                new_client.sock = std::move(socket_client);

                client_list.push_back(new_client);
                std::cout << "nouvelle machine connécté: " << new_client.sock->getRemoteAddress() << std::endl ;

                return true;
            } else return false;
        };

        void getMessages(){
            for (auto& client : client_list){
                
                client.sock->setBlocking(false);

                auto packet = std::make_unique<sf::Packet>();
                Event recu;
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

        void sendReponse(){
            for (auto it = rep_queu.begin(); it != rep_queu.end(); ){
                auto& rep = *it;

                if (rep.synchro == true){
                    for (auto& client : client_list){
                        if(client.sock->getRemoteAddress() != rep.client.sock->getRemoteAddress()){
                            client.sock->send(*(rep.packet));
                        }
                    }
                
                }else{
                    rep.client.sock->send(*(rep.packet));
                }
            it = rep_queu.erase(it);
            };
        };

};



