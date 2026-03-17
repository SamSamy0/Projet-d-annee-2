#pragma once
#include "tool.hpp"
#include "../../client/clientnetwork.hpp"
#include <memory>

class Shift : public Tool {

public:
  Shift(std::shared_ptr<Map> map, ClientNetworkManager& manager);
  void onPress(sf::Vector2i pos) override;
  void onDrag(sf::Vector2i pos) override;
  void onRelease() override;

  virtual ~Shift() = 0;
};
