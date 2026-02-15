#include "client.hpp"

sf::IpAddress Client::getAddress(){
    return sock->getRemoteAddress();
}