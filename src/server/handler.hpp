#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <deque>
#include "protocol.hpp"
#include "displayWindow.hpp"

class ClientNetworkManager;

class ClientEventHandler{
   ClientNetworkManager* manager_;
    public:
    ClientEventHandler(ClientNetworkManager& client_manager);
    void process(ServerEvent& event);
    void trateEventQueu();
};

class HandleRepInWindow{
    Window* window_;
    
    public:
    HandleRepInWindow(Window& w){};
    void switchConnectState(bool connect);
    void addProjectList(std::vector<std::string> project_list);
};



#endif