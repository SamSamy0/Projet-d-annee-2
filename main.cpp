#include "src/displayWindow.hpp"
#include "src/server/clientnetwork.hpp"

int main() {
  ClientNetworkManager manager;
  AuthWindow window(manager);
  window.run();
  return 0;
}
