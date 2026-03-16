#pragma once

#include <memory>
#include <SFML/Network.hpp>
#include "../common/protocol.hpp"


struct ServerEvent{
    MsgProtocole message_type_;
    std::unique_ptr<sf::Packet> data_packet_;
};