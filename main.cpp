#include "src/Window/displayWindow.hpp"
#include "src/server/clientnetwork.hpp"
#include <iostream>

int main() {
  std::cout << "1" << std::endl;
  ClientNetworkManager manager;
  Window window(manager);
  window.run();
  return 0;
}
