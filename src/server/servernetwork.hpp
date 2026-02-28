#pragma once

#include <vector>
#include <deque>
#include <memory>
#include <atomic>
#include <SFML/Network.hpp>
#include "protocol.hpp"
#include "messagequeue.hpp"
#include "reponsequeue.hpp"


class ServerNetworkManager{
    sf::TcpListener listener_;
    std::vector<std::shared_ptr<Client>> client_list_;
    MessageQueue& message_queu_;
    ReponseQueue& rep_queu_;
    std::atomic<bool> m_running_;
    
public:
    ServerNetworkManager(MessageQueue& mes , ReponseQueue& rep);
    bool start();
    bool accept();
    void getMessages();
    void sendReponse();
    void run();
};


