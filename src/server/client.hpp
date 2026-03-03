#pragma once
#include <SFML/Network.hpp>
#include <memory>
#include <atomic>

struct Client{
    std::atomic<long long> id = 0;
    std::shared_ptr<sf::TcpSocket> sock;
    sf::IpAddress getAddress();
};