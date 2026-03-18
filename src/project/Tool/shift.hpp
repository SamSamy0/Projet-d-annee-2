#pragma once
#include "tool.hpp"
#include <memory>

class Shift : public Tool {

public:
  Shift(std::shared_ptr<Map> map, ClientNetworkManager &manager);
  void onPress(sf::Vector2i pos) override;
  void onDrag(sf::Vector2i pos) override;
  void onRelease() override;
  void shift(sf::Vector2i pos);
  void shiftSender(sf::Vector2i delta);

  virtual ~Shift() = 0;
};
