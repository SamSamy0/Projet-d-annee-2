#include "tool.hpp"
#include "layer.hpp"
#include "map.hpp"
#include <memory>

Tool::Tool(std::shared_ptr<Map> map)
    : map_{map}{}
std::shared_ptr<Map> Tool::getMap() { return this->map_; }
unsigned int Tool::getScale() { return map_->getScale(); }

void Brush::setSize(unsigned int x, unsigned int y = 0) {
  size_m_.x = x;
  size_m_.y = y;
}

PixelBrush::PixelBrush(std::shared_ptr<Map> map)
    : Brush(), Tool(map) {name_ = PIXELBRUSH;}

void PixelBrush::setColor(sf::Color c) {
  color_ = c;
} // NOTE: PEUT ETRE FAIRE UNE FONCTION PAR R G B A
void PixelBrush::setShape(Shape s) { shape_ = s; }
void PixelBrush::setErraser() { is_erraser_ = !is_erraser_; }

void PixelBrush::drawOn(Layer &layer, sf::Vector2u pos) {
  PixelLayer *pixellayer = dynamic_cast<PixelLayer *>(&layer); //TODO: gérer l'erreur du cast
  sf::Vector2f rounded_pos = sf::Vector2f(std::round(pos.x), std::round(pos.y));

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
    unsigned int size_x = (size_m_.x) * getScale();
    unsigned int size_y = (size_m_.y) * getScale();
    sf::ConvexShape diamond(4);
    diamond.setPoint(0, sf::Vector2f(size_x / 2, 0));      // top point
    diamond.setPoint(1, sf::Vector2f(size_x / 2, size_y)); // botom point
    diamond.setPoint(2, sf::Vector2f(size_x, size_y / 2)); // right point
    diamond.setPoint(3, sf::Vector2f(size_x / 2, 0));      // left point
    diamond.setOrigin(sf::Vector2f(size_x, size_y));
    diamond.setPosition(rounded_pos);
    is_erraser_ ? diamond.setFillColor(sf::Color::Transparent)
                : diamond.setFillColor(color_);
    is_erraser_ ? pixellayer->errase(diamond) : pixellayer->draw(diamond);

    break;
  }
  }
}

PixelShift::PixelShift(std::shared_ptr<Map> map) : Tool(map){}

void PixelShift::shiftOn(Layer& layer, sf::Vector2u pos_1, sf::Vector2u pos_2){
  PixelLayer *pixellayer = dynamic_cast<PixelLayer *>(&layer); //TODO: gérer l'erreur du cast
  sf::Vector2i vect(static_cast<int>(pos_2.x) - static_cast<int>(pos_1.x),
                    static_cast<int>(pos_2.y) - static_cast<int>(pos_1.y)); //on convertit avant la soustraction
  pixellayer->shift(vect);
}
