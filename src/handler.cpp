#include <SFML/Network.hpp>
#include <iostream>
#include <deque>
#include <memory>
#include "protocol.hpp"
#include "clientnetwork.cpp"

class Handler{
    ClientNetworkManager& manager;
    public:
    Handler(ClientNetworkManager& m): manager(m) {}
    
}