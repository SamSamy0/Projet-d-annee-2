#ifndef REPONSEQUEUE_H
#define REPONSEQUEUE_H

#include <mutex>
#include <condition_variable>
#include <deque>
#include <memory>
#include "client.hpp"
#include "protocol.hpp"


struct Reponse{
    std::shared_ptr<Client> client;
    std::vector<long long> id_list;
    MsgProtocole message_type;
    std::shared_ptr<sf::Packet> packet;
};

class ReponseQueue {
private:
    std::deque<std::shared_ptr<Reponse>> r_queue;
    std::mutex r_mutex;
    std::condition_variable r_cv;
    bool r_stopping = false;

public:
    ReponseQueue() = default;
    ~ReponseQueue() = default;

    void push(std::shared_ptr<Reponse> rps);
    std::shared_ptr<Reponse> pop();
    bool isEmpty();
    void stop();
};

#endif