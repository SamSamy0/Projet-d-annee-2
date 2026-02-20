#include "tool.hpp"
#include "layer.hpp"
#include "map.hpp"
#include <cmath>
#include <memory>

Tool::Tool(std::shared_ptr<Map> map) : map_{map} {}
std::shared_ptr<Map> Tool::getMap() { return this->map_; }
unsigned int Tool::getScale() { return map_->getScale(); }

Brush::Brush(std::shared_ptr<Map> map) : Tool(map) {
  spacing_ = std::min(size_m_.x / 2 * getScale(), size_m_.y / 2 * getScale());
  if (spacing_ < 1)
    spacing_ = 1.0f;
}
void Brush::setSize(unsigned int x, unsigned int y = 0) {
  size_m_.x = x;
  size_m_.y = y;
}

sf::Vector2u Brush::getSize() { return sf::Vector2u(size_m_.x, size_m_.y); }

void Brush::onPress(sf::Vector2i pos) {
  isDrawing_ = true;
  lastPos_ = pos;
  paint(pos);
  distance_ = 0;
}
void Brush::onDrag(sf::Vector2i pos) {
  if (isDrawing_) {
    // On calcule tout en float pour la précision du vecteur
    sf::Vector2f start(lastPos_);
    sf::Vector2f end(pos);
    sf::Vector2f diff = end - start;

    float frameDistance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if (frameDistance > 0) {
      sf::Vector2f unitDir = diff / frameDistance;
      distance_ += frameDistance; // On ajoute la distance parcourue cette frame

      while (distance_ >= spacing_) {
        // On calcule où on doit peindre sur le segment actuel
        // 'travelDist' est la distance depuis 'lastPos_'
        float travelDist = frameDistance - (distance_ - spacing_);
        sf::Vector2f paintPosF = start + (unitDir * travelDist);

        // On convertit en int seulement au moment de peindre
        paint(sf::Vector2i(static_cast<int>(std::round(paintPosF.x)),
                           static_cast<int>(std::round(paintPosF.y))));

        distance_ -= spacing_;
      }
    }
    lastPos_ = pos;
  }
}

void Brush::onRelease() {
  isDrawing_ = false;
  distance_ = 0;
}

PixelBrush::PixelBrush(std::shared_ptr<Map> map) : Brush(map) {
  name_ = PIXELBRUSH;
}

void PixelBrush::setColor(sf::Color c) {
  color_ = c;
} // NOTE: PEUT ETRE FAIRE UNE FONCTION PAR R G B A
void PixelBrush::setShape(Shape s) { shape_ = s; }
void PixelBrush::setErraser(bool val) { is_erraser_ = val; }

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
      is_erraser_ ? diamond.setFillColor(sf::Color::Transparent)
                  : diamond.setFillColor(color_);
      is_erraser_ ? pixellayer->errase(diamond) : pixellayer->draw(diamond);

      break;
    }
    }
  }
}

PixelShift::PixelShift(std::shared_ptr<Map> map) : Tool(map) {
  name_ = PIXELSHIFT;
}

void PixelShift::shiftOn(sf::Vector2i v) {
  std::shared_ptr<Layer> layer = map_->getCurrentLayer();
  layer->shift(v);
}

void PixelShift::onPress(sf::Vector2i pos) {
  isDrawing_ = true;
  lastPos_ = pos;
}
void PixelShift::onDrag(sf::Vector2i pos) {
  if (!isDrawing_)
    return;

  sf::Vector2i delta = pos - lastPos_;

  if (delta.x != 0 || delta.y != 0) {
    std::shared_ptr<Layer> layer = map_->getCurrentLayer();
    layer->shift(delta);
  }
  lastPos_ = pos;
}
void PixelShift::onRelease() {
  if (isDrawing_) {
    std::shared_ptr<Layer> layer = map_->getCurrentLayer();
    if (layer && layer->getType() == PIXELLAYER) {
      layer->display();
    }
  }
  isDrawing_ = false;
}
