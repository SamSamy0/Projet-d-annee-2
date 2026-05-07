#include <iostream>
// #include "../Window/Window.hpp"
#include "../Window/Application.hpp"
#include "clientnetwork.hpp"
#include "handler.hpp"
#include "receiverwindow.hpp"

int main() {
  ClientNetworkManager manager;
  Application window(manager);
  ReceiverInWindow receiver_window(window);
  ClientHandler handler(manager, receiver_window);

  while (window.isOpen()) {
    manager.getEvent();
    handler.processEventQueu();
    window.run();
  }
  return 0;
}
