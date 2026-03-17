#include "client.hpp"
#include <iostream>


sf::IpAddress Client::getAddress() {
    return sock->getRemoteAddress().value_or(sf::IpAddress::Any);
}