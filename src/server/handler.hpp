#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <deque>
#include "clientnetwork.hpp"
#include "protocol.hpp"


class ClientEventHandler{
public:
    void process(ServerRep& event);
};

void handleAuth(ServerRep& event);
void handleProjectData(ServerRep& event);


#endif