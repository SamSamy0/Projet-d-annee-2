#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <deque>
#include "protocol.hpp"
#include "../Window/displayWindow.hpp"

class Window;             // Forward declaration
class HandleRepInWindow;

class ClientNetworkManager;

class ClientEventHandler{
   ClientNetworkManager* manager_;
   HandleRepInWindow* handleWindow_;
    public:
    ClientEventHandler(ClientNetworkManager& client_manager, HandleRepInWindow w);
    void process(ServerEvent& event);
    void trateEventQueu();
};

class HandleRepInWindow{
    Window* window_;
    
    public:
    HandleRepInWindow(Window& w);
    void switchConnectState(uint8_t connect);
    void addProjectList(std::vector<std::string> project_list);
};



#endif