#include "tool.hpp"
#include "../map.hpp"
#include <memory>

Tool::Tool(std::shared_ptr<Map> map, ClientNetworkManager& manager) : map_{map}, manager_{manager} {}
std::shared_ptr<Map> Tool::getMap() { return this->map_; }

unsigned int Tool::getScale() const { return map_->getScale(); }

ToolType Tool::getType() const { return type_; }
