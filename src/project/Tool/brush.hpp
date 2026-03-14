#pragma once
#include "tool.hpp"

class Brush : public Tool {
protected:
  sf::Vector2f size_m_ = sf::Vector2f(1, 1);
  int distance_ = 0;
  float spacing_;

public:
  Brush(std::shared_ptr<Map>);
  virtual void setSize(float x, float y) =0;
  sf::Vector2f getSize();
  void onPress(sf::Vector2i pos);
  void onDrag(sf::Vector2i pos);
  void onRelease();
  virtual void paint(sf::Vector2i pos) = 0;
  virtual ~Brush() = default;
};
