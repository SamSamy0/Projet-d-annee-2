#include "../Window/Window.hpp"
#include "clientnetwork.hpp"
#include "handler.hpp"
#include "receiverwindow.hpp"


int main() {
  ClientNetworkManager manager;
  Window window(manager);
  ReceiverInWindow receiver_window(window);
  ClientHandler handler(manager,receiver_window);
  
  manager.connect();
  while (window.isOpen()) {
    manager.getEvent();
    handler.processEventQueu();
    window.run();
  }
  window.run();
  return 0;
}
