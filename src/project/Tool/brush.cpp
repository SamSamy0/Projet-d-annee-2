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
  if (isDrawing_) {
    // On calcule tout en float pour la précision du vecteur
    sf::Vector2f start(lastPos_);
    sf::Vector2f end(pos);
    sf::Vector2f diff = end - start;

    float pixelDistance = std::sqrt(diff.x * diff.x + diff.y * diff.y); // quand on applique le vecteur diff on parcours pixelDistance

    if (pixelDistance > 0) {
      sf::Vector2f dirrection = diff / pixelDistance; //vecteur de longueur 1 qui sert à connetre la dirrection en fonction du signe de x et y
      distance_ += pixelDistance; // On ajoute la distance parcourue cette frame

      while (distance_ >= spacing_) {
        // On calcule où on doit peindre sur le segment actuel
        // 'travelDist' est la distance depuis 'lastPos_'
        float travelDist = pixelDistance - (distance_ - spacing_);
        sf::Vector2f paintPosF = start + (dirrection * travelDist);

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

