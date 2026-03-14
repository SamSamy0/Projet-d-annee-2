#pragma once
#include "tool.hpp"
#include <memory>

class NoneTool : public Tool {
public:
  NoneTool(std::shared_ptr<Map> map);
  void onPress(sf::Vector2i pos);
  void onDrag(sf::Vector2i pos);
  void onRelease();
};
