#pragma once
#include "layer.hpp"
#include <cstdint>

struct SpriteObject {
  sf::Sprite sprite;
  uint id;
  SpriteObject(const sf::Sprite &sprite, uint id);
};

class SpriteLayer : public Layer {
  std::vector<SpriteObject> sprites_;
  sf::Vector2i offset_;
  uint nextId_;

public:
  SpriteLayer(uint id, std::string name, sf::Vector2u size);
  void setNextId(uint);
  std::vector<SpriteObject> &getSprites();
  sf::Vector2i getOffset() const override;
  void draw(const sf::Sprite &s);
  void draw(std::shared_ptr<SpriteLayer> layer);
  void erase(uint id);
  void shift(sf::Vector2i v) override;
  void shiftSprite(uint id, sf::Vector2i v);
  void drawLayer(sf::RenderTarget &target) override;

  void resizeSprite(uint id, sf::Vector2f pos, float scale);
  void rotateSprite(uint id, float angle, sf::Vector2f pos);
};
