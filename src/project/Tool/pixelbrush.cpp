#include "pixelbrush.hpp"
#include "../Layer/layer.hpp"
#include "../Layer/pixellayer.hpp"
#include "../map.hpp"
#include <memory>

PixelBrush::PixelBrush(std::shared_ptr<Map> map) : Brush(map) {
  type_ = PIXELBRUSH;
}





void PixelBrush::setColor(sf::Color c) {color_ = c;} 
void PixelBrush::setShape(Shape s) { shape_ = s; }
void PixelBrush::setEraser(bool val) { is_eraser_ = val; }


  bool PixelBrush::getEraser() const{
  return is_eraser_;
}
Shape PixelBrush::getShape()const {
  return shape_;
}

sf::Color PixelBrush::getColor()const{
  return color_;
}

void PixelBrush::paintSender(sf::Vector2i pos){
  //manager.message
  paint(pos);
}

void PixelBrush::paint(sf::Vector2i pos) {
  /* Draw a shape one the layer*/
  shared_ptr<Layer> layer = map_->getCurrentLayer();
  if (!layer || layer->getType() != PIXELLAYER)
    return;

  shared_ptr<PixelLayer> pixellayer = (static_pointer_cast<PixelLayer>(layer));

  sf::Vector2i shifted_pos = sf::Vector2i(pos.x - pixellayer->getOffset().x,
                                          pos.y - pixellayer->getOffset().y);

  sf::Vector2f rounded_pos = sf::Vector2f((shifted_pos.x), (shifted_pos.y));

  switch (shape_) {
  case SQUARE: {
    unsigned int size = (size_m_.x) * getScale();
    sf::RectangleShape square(sf::Vector2f(size, size));
    unsigned int offset = size / 2;
    square.setOrigin(sf::Vector2f(offset, offset));
    square.setPosition(rounded_pos);
    is_eraser_ ? square.setFillColor(sf::Color::Transparent)
               : square.setFillColor(color_);
    is_eraser_ ? pixellayer->erase(square) : pixellayer->draw(square);
    break;
  }
  case CIRCLE: {
    unsigned int size = (size_m_.x) * getScale();
    unsigned int radius = size / 2;
    sf::CircleShape circle(radius);
    circle.setOrigin(sf::Vector2f(radius, radius));
    circle.setPosition(rounded_pos);
    is_eraser_ ? circle.setFillColor(sf::Color::Transparent)
               : circle.setFillColor(color_);
    is_eraser_ ? pixellayer->erase(circle) : pixellayer->draw(circle);
    break;
  }
  case DIAMOND: {
    pixellayer->getTexture().setSmooth(false);
    unsigned int size_x = (size_m_.x) * getScale();
    unsigned int size_y = (size_m_.y) * getScale();
    sf::ConvexShape diamond(4);
    diamond.setPoint(0, sf::Vector2f(size_x / 2.0f, 0.0f));   // top point
    diamond.setPoint(1, sf::Vector2f(size_x, size_y / 2.0f)); // right point
    diamond.setPoint(2, sf::Vector2f(size_x / 2.0f, size_y)); // botom point
    diamond.setPoint(3, sf::Vector2f(0.0f, size_y / 2.0f));   // left point
    diamond.setOrigin(sf::Vector2f(size_x / 2.0f, size_y / 2.0f));
    diamond.setPosition(rounded_pos);
    is_eraser_ ? diamond.setFillColor(sf::Color::Transparent)
               : diamond.setFillColor(color_);
    is_eraser_ ? pixellayer->erase(diamond) : pixellayer->draw(diamond);

    break;
  }
  }
}
