#pragma once
#include "brush.hpp"
#include "../../client/clientnetwork.hpp"

class SpriteEraser : public Brush {
  Shape shape_ = SQUARE;

public:
  SpriteEraser(std::shared_ptr<Map> map, ClientNetworkManager &manager);
  void setShape(Shape s);
  Shape getShape() const;
  bool checkColision(sf::Vector2i pos, sf::FloatRect r);
  int paint(sf::Vector2i pos);
  void paintSender(sf::Vector2i pos) override;
};
