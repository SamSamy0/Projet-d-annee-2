#pragma once 
#include "layer.hpp"


class SpriteLayer : public Layer {
  std::vector<sf::Sprite> sprites_;

public:
  SpriteLayer(std::string name, sf::Vector2u size); //TODO: à définir
  LayerContent getLayerContent() override;
  void drawLayer(sf::RenderTarget& target) override;
  void draw(sf::Drawable &s) override;
  void errase(sf::Drawable &s) override;
  void shift(sf::Vector2i v)override;
  void display() override;
};
