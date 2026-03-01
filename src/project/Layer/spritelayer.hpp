#pragma once 
#include "layer.hpp"


class SpriteLayer : public Layer {
  std::vector<sf::Sprite> sprites_;

public:
  SpriteLayer(); // TODO: à définir
  void drawLayer(sf::RenderWindow& window) override;
  void draw(sf::Drawable &s) override;
  void errase(sf::Drawable &s) override;
  void shift(sf::Vector2i v)override;
  void display() override;
};
