#pragma once
#include "layer.hpp"
#include <cstdint>
#include <unordered_map>

struct SpriteObject {
  sf::Sprite sprite;
  uint id;
  std::string assetId;
  SpriteObject(const sf::Sprite &sprite, uint id, std::string assetId = "");
};

class SpriteLayer : public Layer {
  // std::vector<SpriteObject> sprites_;
  std::unordered_map<uint, SpriteObject> sprites_;
  sf::Vector2i offset_;
  uint nextId_;

public:
  SpriteLayer(uint id, std::string name, sf::Vector2u size);
  void setNextId(uint);
  std::unordered_map<uint, SpriteObject>& getSprites();
  SpriteObject& getSprite(uint id);
  sf::Vector2i getOffset() const override;
  void draw(const sf::Sprite &s, std::string assetId = "");
  void draw(std::shared_ptr<SpriteLayer> layer);
  void erase(uint id);
  void shift(sf::Vector2i v) override;
  void shiftSprite(uint id, sf::Vector2i v);
  void drawLayer(sf::RenderTarget &target) override;

  void addSprite(const sf::Sprite &s, uint id);

  void resizeSprite(uint id, sf::Vector2f pos, float scale);
  void rotateSprite(uint id, float angle, sf::Vector2f pos);
};
