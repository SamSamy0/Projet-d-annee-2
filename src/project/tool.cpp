#include "tool.hpp"
#include "layer.hpp"
#include "map.hpp"
#include <memory>

Tool::Tool(std::shared_ptr<Map> map) : map_{map}{}
std::shared_ptr<Map> Tool::getMap() { return this->map_; }
unsigned int Tool::getScale() { return map_->getScale(); }

Brush::Brush(std::shared_ptr<Map> map) : Tool(map){
 spacing_ = std::min(size_m_.x/2*getScale(),size_m_.y/2*getScale()); 
}
void Brush::setSize(unsigned int x, unsigned int y = 0) {
  size_m_.x = x;
  size_m_.y = y;
}



void Brush::drawOn(sf::Vector2i pos){
  if(!isDrawing_){
    isDrawing_ = true;
    lastPos_ = pos;
    paint(pos);
  }

  else{

    sf::Vector2i temp = pos-lastPos_;
    int distance = sqrt(temp.x*temp.x+temp.y*temp.y);
    distance_ += distance;



  }

}

PixelBrush::PixelBrush(std::shared_ptr<Map> map) :Brush(map) {
  name_ = PIXELBRUSH;
  }


void PixelBrush::setColor(sf::Color c) {
  color_ = c;
} // NOTE: PEUT ETRE FAIRE UNE FONCTION PAR R G B A
void PixelBrush::setShape(Shape s) { shape_ = s; }
void PixelBrush::setErraser() { is_erraser_ = !is_erraser_; }

void PixelBrush::paint(sf::Vector2i pos) {

  shared_ptr<Layer> pixellayer = map_->getCurrentLayer() ;
  if (pixellayer->getType() == PIXELLAYER){
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
      pixellayer->getTexture().setSmooth(false);
      unsigned int size_x = (size_m_.x) * getScale();
      unsigned int size_y = (size_m_.y) * getScale();
      sf::ConvexShape diamond(4);
      diamond.setPoint(0, sf::Vector2f(size_x / 2.0f, 0.0f));      // top point
        diamond.setPoint(1, sf::Vector2f(size_x, size_y / 2.0f)); // right point
      diamond.setPoint(2, sf::Vector2f(size_x / 2.0f, size_y)); // botom point
      diamond.setPoint(3, sf::Vector2f(0.0f, size_y/2.0f));      // left point
      diamond.setOrigin(sf::Vector2f(size_x/2.0f, size_y/2.0f));
      diamond.setPosition(rounded_pos);
      is_erraser_ ? diamond.setFillColor(sf::Color::Transparent)
                  : diamond.setFillColor(color_);
      is_erraser_ ? pixellayer->errase(diamond) : pixellayer->draw(diamond);

      break;
    }
    }
  }
}

PixelShift::PixelShift(std::shared_ptr<Map> map) : Tool(map){name_ = PIXELSHIFT;}

void PixelShift::shiftOn(sf::Vector2i pos_1, sf::Vector2i pos_2){
  std::shared_ptr<Layer> layer = map_->getCurrentLayer();
  if (layer->getType() == PIXELLAYER){
    sf::Vector2i vect(static_cast<int>(pos_2.x) - static_cast<int>(pos_1.x),
                      static_cast<int>(pos_2.y) - static_cast<int>(pos_1.y)); //on convertit avant la soustraction
    layer->shift(vect);
  }
}
