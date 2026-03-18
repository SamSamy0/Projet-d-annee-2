#pragma once
#include "brush.hpp"

class PixelBrush : public Brush {
  sf::Color color_ = sf::Color::Black;
  Shape shape_ = SQUARE;
  bool is_eraser_ = false;

public:
  PixelBrush(std::shared_ptr<Map> map, ClientNetworkManager &manager);
  void setColor(sf::Color c);
  void setShape(Shape s);
  void paint(sf::Vector2i pos) override;
  void paintSender(sf::Vector2i pos) override;
  bool getEraser() const;
  sf::Color getColor() const;
  Shape getShape() const;
  void setEraser(bool val);
};
