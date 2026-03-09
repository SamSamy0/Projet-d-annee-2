#include "spritelayer.hpp"




SpriteLayer::SpriteLayer(std::string name, sf::Vector2u size): Layer(name,size){}

 

sf::Vector2i SpriteLayer::getOffset(){return offset_;}

void SpriteLayer::draw(sf::Sprite &s){
  sprites_.push_back(s);
}

void SpriteLayer::shift(sf::Vector2i v) {offset_ += v;}


void SpriteLayer::drawLayer(sf::RenderTarget& target) {
  if (!masked_) {
    sf::RenderStates state;
    state.transform.translate(sf::Vector2f(static_cast<float>(offset_.x),static_cast<float>(offset_.y)));
    for(const sf::Sprite& sprite : sprites_){
      target.draw(sprite,state);
    }
  }
}
