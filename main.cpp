#include "src/Window/displayWindow.hpp"
#include "src/client/clientnetwork.hpp"
#include <iostream>
#include "src/client/handler.hpp"

int main() {
  std::cout << "1" << std::endl;
  ClientNetworkManager manager;
  Window window(manager);
  HandleRepInWindow handler_window(window);
  std::shared_ptr<ClientEventHandler> handler_client = std::make_shared<ClientEventHandler>(manager, handler_window);
  window.set_handler(handler_client);


  window.run();
  return 0;
}
