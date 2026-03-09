#pragma once 
#include "layer.hpp"

class PixelLayer : public Layer {

  sf::RenderTexture texture_;
  sf::Sprite offset_;

public:
  PixelLayer(std::string name, sf::Vector2u size);

  sf::Vector2i getOffset() const;
  sf::RenderTexture& getTexture();
  void draw(sf::Drawable &s);
  void erase(sf::Drawable &s) override;
  void shift(sf::Vector2i v)override;
  void drawLayer(sf::RenderTarget& target) override;
  void display();
};
