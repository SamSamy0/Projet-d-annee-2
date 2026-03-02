#pragma once
#include <SFML/Network.hpp>
#include <memory>



//cette structure represente les client dans la liste des clients connécter au serveur
struct Client{
    long long id = 0;
    std::shared_ptr<sf::TcpSocket> sock;
    
    sf::IpAddress getAddress();
};