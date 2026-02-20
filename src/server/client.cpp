#include "client.hpp"
#include <iostream>

sf::IpAddress Client::getAddress() {
    std::cout << "Entrée dans getAdresse" << std::endl;
    return sock->getRemoteAddress().value_or(sf::IpAddress::Any);
}