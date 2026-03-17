#pragma once
#include "layer.hpp"

class SpriteLayer : public Layer {
  std::vector<sf::Sprite> sprites_;
  sf::Vector2i offset_;

public:
  SpriteLayer(uint64_t id, std::string name, sf::Vector2u size);
  sf::Vector2i getOffset() const override;
  std::vector<sf::Sprite> &getSprites();
  void draw(sf::Sprite &s);
  void erase(int i);
  void shift(sf::Vector2i v) override;
  void drawLayer(sf::RenderTarget &target) override;
};
