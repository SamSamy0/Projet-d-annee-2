#pragma once 
#include "brush.hpp"


class PixelBrush :public Brush {
  sf::Color color_ = sf::Color::Black;
  Shape shape_ = SQUARE;
  bool is_erraser_ = false;

public:
  PixelBrush(std::shared_ptr<Map> map);
  void setColor(sf::Color c);
  void setShape(Shape s);
  void paint(sf::Vector2i pos);
  void setErraser(bool val);
  bool getErraser() const;
};
