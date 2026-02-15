#include <SFML/Network.hpp>
#include <memory>

struct Client{
    long long id = 0;
    std::shared_ptr<sf::TcpSocket> sock;
    
    sf::IpAddress getAddress();
};