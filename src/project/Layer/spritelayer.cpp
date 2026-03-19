#include "spritelayer.hpp"

SpriteLayer::SpriteLayer(uint id, std::string name, sf::Vector2u size)
    : Layer(id,name, size) {
  type_ = SPRITELAYER;
}



SpriteObject::SpriteObject(const sf::Sprite& sprite, uint id): sprite{sprite},id{id}{}

sf::Vector2i SpriteLayer::getOffset() const { return offset_; }

const std::vector<SpriteObject> &SpriteLayer::getSprites() const { return sprites_; }

void SpriteLayer::draw(const sf::Sprite &s) {
  sprites_.push_back(SpriteObject(s,nextId_));
  nextId_ ++;}

void SpriteLayer::erase(uint id) {
  for(auto i = sprites_.begin();i!=sprites_.end();i++){
    if(i->id == id){
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
