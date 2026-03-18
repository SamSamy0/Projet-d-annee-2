#include "nonetool.hpp"

NoneTool::NoneTool(std::shared_ptr<Map> map, ClientNetworkManager &manager) : Tool(map, manager){}
void NoneTool::onPress(sf::Vector2i pos) {}
void NoneTool::onDrag(sf::Vector2i pos) {}
void NoneTool::onRelease() {}
