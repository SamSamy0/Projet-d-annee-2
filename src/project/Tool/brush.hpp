#pragma once 
#include "tool.hpp"

class Brush :public Tool {
protected:
  sf::Vector2u size_m_ = sf::Vector2u(1, 1);
  int distance_ = 0.0f;
  int spacing_;
public:
  Brush(std::shared_ptr<Map>);
  void setSize(unsigned int x, unsigned int y);
  sf::Vector2u getSize();
  void onPress(sf::Vector2i pos);
  void onDrag(sf::Vector2i pos);
  void onRelease();
  virtual void paint(sf::Vector2i pos) = 0;
  virtual ~Brush() = default;
};
