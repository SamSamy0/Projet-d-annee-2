#pragma once
#include <SFML/Network.hpp>
#include <memory>
#include <atomic>



//cette structure represente les client dans la liste des clients connécter au serveur
struct Client{
    std::atomic<uint> projectId = 0;
    std::atomic<uint> id = 0;
    std::shared_ptr<sf::TcpSocket> sock;
    sf::IpAddress getAddress();
    std::string pseudo = "adam";
};