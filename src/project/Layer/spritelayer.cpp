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

std::vector<SpriteObject> &SpriteLayer::getSprites() { return sprites_; }

void SpriteLayer::draw(const sf::Sprite &s) {
  sprites_.push_back(SpriteObject(s, nextId_));
  nextId_++;
}

void SpriteLayer::erase(uint id) {
  for (auto i = sprites_.begin(); i != sprites_.end(); i++) {
    if (i->id == id) {
      sprites_.erase(i);
      break;
    }
  }
}

void SpriteLayer::shift(sf::Vector2i v) { offset_ += v; }

void SpriteLayer::drawLayer(sf::RenderTarget &target) {
  if (!masked_) {
    sf::RenderStates state;
    state.transform.translate(sf::Vector2f(static_cast<float>(offset_.x),
                                           static_cast<float>(offset_.y)));
    for (const SpriteObject &spriteobject : sprites_) {
      target.draw(spriteobject.sprite, state);
    }
  }
}


void SpriteLayer::draw(std::shared_ptr<SpriteLayer> layer){
  std::vector<SpriteObject> vector = layer->sprites_;
  for(int i = 0 ; i < vector.size();i++){
    draw(vector[i].sprite);
  }
}

void SpriteLayer::setNextId(uint nextId) { nextId_ = nextId; }

void SpriteLayer::resizeSprite(uint spriteId, sf::Vector2f pos,
                               float scale) {
  for (int i = sprites_.size() - 1; i >= 0;i--) {
    if (sprites_[i].id == spriteId){
      sprites_[i].sprite.setPosition(pos);
      sprites_[i].sprite.scale(sf::Vector2f(scale, scale));
      break;

    }
  }
}

void SpriteLayer::rotateSprite(uint spriteId, float angle,
                               sf::Vector2f pos) {

  for (int i = sprites_.size() - 1; i >= 0;i--) {
    if (sprites_[i].id == spriteId){
      sprites_[i].sprite.setPosition(pos);
      sprites_[i].sprite.rotate(sf::radians(angle));
      break;

    }
  }
}

void SpriteLayer::shiftSprite(uint id, sf::Vector2i v) {
  for (int i = sprites_.size() - 1; i >= 0; i--) {
    if (sprites_[i].id == id) {
      sprites_[i].sprite.move(sf::Vector2f(v.x, v.y));
      break;
    }
  }
}
