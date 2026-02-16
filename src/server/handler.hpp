#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <deque>
#include "clientnetwork.hpp"
#include "protocol.hpp"


class ClientEventHandler{
    ClientNetworkManager* manager_;
    public:
    ClientEventHandler(ClientNetworkManager& client_manageer);
    void process(ServerEvent& event);
    void trateEventQueu();
};

void handleAuth(ServerEvent& event);
void handleProjectData(ServerEvent& event);


#endif