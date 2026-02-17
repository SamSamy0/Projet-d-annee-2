#include "src/Window/displayWindow.hpp"
#include "src/server/clientnetwork.hpp"

int main() {
  ClientNetworkManager manager;
  Window window(manager);
  window.run();
  return 0;
}
