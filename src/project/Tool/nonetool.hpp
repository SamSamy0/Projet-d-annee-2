#pragma once
#include "tool.hpp"
#include <memory>
#include "../../client/clientnetwork.hpp"

class NoneTool : public Tool {
public:
  NoneTool(std::shared_ptr<Map> map, ClientNetworkManager &manager);
  void onPress(sf::Vector2i pos);
  void onDrag(sf::Vector2i pos);
  void onRelease();
};
