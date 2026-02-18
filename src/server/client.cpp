#include "client.hpp"

sf::IpAddress Client::getAddress() {
    // getRemoteAddress() retourne maintenant un std::optional<sf::IpAddress>
    auto address = sock->getRemoteAddress();

    // On vérifie si l'adresse existe, sinon on retourne une adresse par défaut (Any)
    if (address.has_value()) {
        return address.value();
    }

    return sf::IpAddress::Any; 
}