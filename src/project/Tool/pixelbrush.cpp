#include "pixelbrush.hpp"
#include "../Layer/layer.hpp"
#include "../map.hpp"
#include <memory>



PixelBrush::PixelBrush(std::shared_ptr<Map> map) : Brush(map) {
  name_ = PIXELBRUSH;
}

void PixelBrush::setColor(sf::Color c) {
  color_ = c;
} // NOTE: PEUT ETRE FAIRE UNE FONCTION PAR R G B A
void PixelBrush::setShape(Shape s) { shape_ = s; }
void PixelBrush::setErraser(bool val) { is_erraser_ = val; }


  bool PixelBrush::getErraser() const{
  return is_erraser_;
}

void PixelBrush::paint(sf::Vector2i pos) {
  shared_ptr<Layer> pixellayer = map_->getCurrentLayer();
  if (pixellayer->getType() == PIXELLAYER) {
    sf::Vector2f rounded_pos = sf::Vector2f((pos.x), (pos.y));

    switch (shape_) {
    case SQUARE: {
      unsigned int size = (size_m_.x) * getScale();
      sf::RectangleShape square(sf::Vector2f(size, size));
      unsigned int offset = size / 2;
      square.setOrigin(sf::Vector2f(offset, offset));
      square.setPosition(rounded_pos);
      is_erraser_ ? square.setFillColor(sf::Color::Transparent)
                  : square.setFillColor(color_);
      is_erraser_ ? pixellayer->errase(square) : pixellayer->draw(square);
      break;
    }
    case CIRCLE: {
      unsigned int size = (size_m_.x) * getScale();
      unsigned int radius = size / 2;
      sf::CircleShape circle(radius);
      circle.setOrigin(sf::Vector2f(radius, radius));
      circle.setPosition(rounded_pos);
      is_erraser_ ? circle.setFillColor(sf::Color::Transparent)
                  : circle.setFillColor(color_);
      is_erraser_ ? pixellayer->errase(circle) : pixellayer->draw(circle);
      break;
    }
    case DIAMOND: {
      // pixellayer->getLayerContent().setSmooth(false);
      unsigned int size_x = (size_m_.x) * getScale();
      unsigned int size_y = (size_m_.y) * getScale();
      sf::ConvexShape diamond(4);
      diamond.setPoint(0, sf::Vector2f(size_x / 2.0f, 0.0f));   // top point
      diamond.setPoint(1, sf::Vector2f(size_x, size_y / 2.0f)); // right point
      diamond.setPoint(2, sf::Vector2f(size_x / 2.0f, size_y)); // botom point
      diamond.setPoint(3, sf::Vector2f(0.0f, size_y / 2.0f));   // left point
      diamond.setOrigin(sf::Vector2f(size_x / 2.0f, size_y / 2.0f));
      diamond.setPosition(rounded_pos);
      is_erraser_ ? diamond.setFillColor(sf::Color::Transparent)
                  : diamond.setFillColor(color_);
      is_erraser_ ? pixellayer->errase(diamond) : pixellayer->draw(diamond);

      break;
    }
    }
  }
}

