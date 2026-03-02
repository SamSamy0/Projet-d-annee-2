#ifndef SERVERNETWORK_HPP
#define SERVERNETWORK_HPP

#include <vector>
#include <memory>
#include <atomic>
#include <SFML/Network.hpp>
#include "protocol.hpp"
#include "reponse/reponse.hpp"
#include "message/message.hpp"
#include "mutexqueue.hpp"
#include "client.hpp"
#include <unordered_map>
#include <thread>


class ServerNetworkManager{
    sf::TcpListener listener_;
    sf::SocketSelector selector_;
    std::thread listenThread_;
    MutexQueue<IMessage>& messageQueu_;
    MutexQueue<Reponse>& repQueu_;
    std::atomic<bool> mRunning_;
    std::vector<std::shared_ptr<Client>> client_list;


    void listen();
    void handleNewConnection();
    void handleClientMessages();
    void stop();
    bool start();
    bool accept();
    void getMessages();
    void sendReponse();
public:
    ServerNetworkManager(MutexQueue<IMessage>& mes , MutexQueue<Reponse>& rep);

    void run();

    std::unordered_map<long long, std::shared_ptr<Client>> map_;
};

#endif