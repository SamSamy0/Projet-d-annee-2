#ifndef SERVERNETWORK_HPP
#define SERVERNETWORK_HPP

#include <vector>
#include <deque>
#include <memory>
#include <atomic>
#include <SFML/Network.hpp>
#include "protocol.hpp"
#include "messagequeue.hpp"
#include "reponsequeue.hpp"


class ServerNetworkManager{
    sf::TcpListener listener;
    std::vector<std::shared_ptr<Client>> client_list;
    MessageQueue& message_queu;
    ReponseQueue& rep_queu;
    std::atomic<bool> m_running;
    
public:
    ServerNetworkManager(MessageQueue& mes , ReponseQueue& rep);
    bool start();
    bool accept();
    void getMessages();
    void sendReponse();
    void run();
};

void debugAuthPacket(sf::Packet copy) ;

#endif