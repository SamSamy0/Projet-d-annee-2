#include <memory>
#include "tool.hpp"
#include "../map.hpp"

Tool::Tool(std::shared_ptr<Map> map) : map_{map} {}
std::shared_ptr<Map> Tool::getMap() { return this->map_; }
unsigned int Tool::getScale() { return map_->getScale(); }

