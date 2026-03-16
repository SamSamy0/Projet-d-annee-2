#include "nonetool.hpp"

NoneTool::NoneTool(std::shared_ptr<Map> map) : Tool(map){}
void NoneTool::onPress(sf::Vector2i pos) {}
void NoneTool::onDrag(sf::Vector2i pos) {}
void NoneTool::onRelease() {}
