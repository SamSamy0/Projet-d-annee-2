#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <deque>
#include "clientnetwork.hpp"


class ClientEventHandler{
public:
    void process(ServerRep& event);
};

#endif