#include "spritelayer.hpp"
#include <SFML/System/Angle.hpp>
#include <cmath>

SpriteLayer::SpriteLayer(uint id, std::string name, sf::Vector2u size)
    : Layer(id, name, size) {
  type_ = SPRITELAYER;
  nextId_ = 0;
}

SpriteObject::SpriteObject(const sf::Sprite &sprite, uint id)
    : sprite{sprite}, id{id} {}

sf::Vector2i SpriteLayer::getOffset() const { return offset_; }

const std::unordered_map<uint, SpriteObject>& SpriteLayer::getSprites() const { return sprites_; }

SpriteObject& SpriteLayer::getSprite(uint id){return sprites_.at(id);}

void SpriteLayer::draw(const sf::Sprite &s) {
  sprites_.emplace(nextId_, SpriteObject(s,nextId_));
  nextId_++;
}

void SpriteLayer::erase(uint id) {
  sprites_.erase(id);
}

void SpriteLayer::shift(sf::Vector2i v) { offset_ += v; }

void SpriteLayer::drawLayer(sf::RenderTarget &target) {
  if (!masked_) {
    sf::RenderStates state;
    state.transform.translate(sf::Vector2f(static_cast<float>(offset_.x),
                                           static_cast<float>(offset_.y)));
    for (const auto &[id, spriteObj] : sprites_) {
      target.draw(spriteObj.sprite, state);
    }
  }
}

void SpriteLayer::addSprite(const sf::Sprite &s, uint id) {
  sprites_.emplace(id, SpriteObject(s, id));
}

void SpriteLayer::setNextId(uint nextId) { nextId_ = nextId; }

void SpriteLayer::resizeSprite(uint spriteId, sf::Vector2f pos,
                               float scale) {
  sf::Sprite &sprite = getSprite(spriteId).sprite;
  sprite.setPosition(pos);
  sprite.scale(sf::Vector2f(scale, scale));

}

void SpriteLayer::rotateSprite(uint spriteId, float angle,
                               sf::Vector2f pos) {
  sf::Sprite &sprite = getSprite(spriteId).sprite;
  sprite.setPosition(pos);
  sprite.rotate(sf::radians(angle));
}

void SpriteLayer::shiftSprite(uint id, sf::Vector2i v) {
  getSprite(id).sprite.move(sf::Vector2f(v.x, v.y));
}
