#include "../Window/Window.hpp"
#include "../client/clientnetwork.hpp"

int main() {
  ClientNetworkManager manager;
  Window window(manager);
  while (window.isOpen()) {
    window.run();
  }
  window.run();
  return 0;
}
