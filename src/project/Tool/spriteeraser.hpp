#pragma once
#include "brush.hpp"

class SpriteEraser : public Brush {
  Shape shape_ = SQUARE;

public:
  SpriteEraser(std::shared_ptr<Map> map);
  void setShape(Shape s);
  bool checkColision(sf::Vector2i pos, sf::FloatRect r);
  void paint(sf::Vector2i pos) override;
  void paintSender(sf::Vector2i pos) override {
  } // Pour l'instant elle fait rien
};
