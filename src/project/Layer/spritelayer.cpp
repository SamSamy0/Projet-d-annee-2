#include "spritelayer.hpp"




SpriteLayer::SpriteLayer(std::string name, sf::Vector2u size): Layer(name,size){}

void SpriteLayer::drawLayer(sf::RenderTarget& target) {
  if (!masked_) {
    for(const sf::Sprite& sprite : sprites_){
      target.draw(sprite);
    }
  }
}
 
