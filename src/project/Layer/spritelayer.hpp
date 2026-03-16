#pragma once
#include "layer.hpp"
#include <cstdint>

struct SpriteObject{
  sf::Sprite sprite;
  uint64_t id;
  SpriteObject(const sf::Sprite& sprite, uint64_t id);
};

class SpriteLayer : public Layer {
  std::vector<SpriteObject> sprites_;
  sf::Vector2i offset_;
  uint64_t nextId;

public:
  SpriteLayer(std::string name, sf::Vector2u size);
  sf::Vector2i getOffset() const override;
  const std::vector<SpriteObject> &getSprites()const;
  void draw(const sf::Sprite &s);
  void erase(uint64_t id);
  void shift(sf::Vector2i v) override;
  void drawLayer(sf::RenderTarget &target) override;
};
