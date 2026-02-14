#ifndef SERVERNETWORK_HPP
#define SERVERNETWORK_HPP

#include <vector>
#include <deque>
#include <memory>
#include <SFML/Network.hpp>
#include "protocol.hpp"
#include "messages.cpp"



struct Reponse{
    Client client;
    std::vector<int> id_list;
    MsgProtocole message_type;
    std::unique_ptr<sf::Packet> packet;
};


class ServerNetworkManager{
    sf::TcpListener listener;
    std::vector<Client> client_list;
    std::deque<std::unique_ptr<Message>> message_queu;
    std::deque<Reponse> rep_queu;
    
public:
    bool start();
    bool accept();
    void getMessages();
    void sendReponse();
};

#endif