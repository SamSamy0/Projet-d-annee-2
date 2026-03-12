#pragma once

#include <deque>
#include "protocol.hpp"
#include "receiverwindow.hpp"
#include "../Window/Window.hpp"


class Window;             // Forward declaration
class HandleRepInWindow;
class ClientNetworkManager;

class ClientHandler{
   ClientNetworkManager& manager_;
    ReceiverInWindow& handleWindow_;

public:
    ClientHandler(ClientNetworkManager& client_manager,ReceiverInWindow& w);
    void process(ServerEvent& event);
    void processEventQueu();
};


