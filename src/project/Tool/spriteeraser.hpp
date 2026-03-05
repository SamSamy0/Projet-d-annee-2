#pragma once
#include "brush.hpp"






class SpriteEraser : public Brush{

  Shape shape_ = SQUARE;

public:
  SpriteEraser(std::shared_ptr<Map> map);
  void paint(sf::Vector2i pos);
};
