#pragma once

#include <deque>
#include "../common/protocol.hpp"
#include "../Window/Application.hpp"
#include "receiverwindow.hpp"
// // #include "../Window/Window.hpp"
#include "../project/Tool/tool.hpp"


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


