#include "brush.hpp"
#include <cmath>

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
  /*Interpolation Function */
  if (isDrawing_) {
    // On calate in float for the precision
    sf::Vector2f start(lastPos_);
    sf::Vector2f end(pos);
    sf::Vector2f diff = end - start;

    // When we apply the diff vector, we travel pixelDistance
    float pixelDistance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    if (pixelDistance > 0) {
      sf::Vector2f dirrection = diff / pixelDistance; // size's vector = 1
      distance_ += pixelDistance;                     // We scale the distance

      while (distance_ >= spacing_) {
        // We calculate where we have to paint on the current segment
        // 'travelDist' is the distance from 'lastPos'
        float travelDist = pixelDistance - (distance_ - spacing_);
        sf::Vector2f paintPosF = start + (dirrection * travelDist);

        // We only convert to int when painting
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
