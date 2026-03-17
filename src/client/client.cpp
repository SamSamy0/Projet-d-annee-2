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

  if (manager.connect()) {
    while (window.isOpen()) {
      manager.getEvent();
      handler.processEventQueu();
      window.run();
    }
  } else {
    std::cout << "conexion au serveur impossible" << std::endl;
  }
  return 0;
}
