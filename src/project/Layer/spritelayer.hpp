#pragma once 
#include "layer.hpp"


class SpriteLayer : public Layer {
  std::vector<sf::Sprite> sprites_;
  sf::Vector2i offset_;

public:
  SpriteLayer(std::string name, sf::Vector2u size);
  void drawLayer(sf::RenderTarget& target) override;
  void draw(sf::Drawable &s) override{}
  void erase(sf::Drawable &s) override{}
  void shift(sf::Vector2i v)override;
  void display() override{}
};
