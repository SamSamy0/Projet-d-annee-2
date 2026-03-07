#include "pixellayer.hpp"

PixelLayer::PixelLayer(std::string name,sf::Vector2u size) : Layer(name, size), offset_(texture_.getTexture()) {
  if(!texture_.resize(size)){
    // NOTE:GERER L'ERREUR 
    }
    offset_.setTexture(texture_.getTexture(),true);
    texture_.clear(sf::Color::Transparent);
    texture_.display();
    type_ = PIXELLAYER;
}
LayerContent PixelLayer::getLayerContent() { return &texture_; }
void PixelLayer::draw(sf::Drawable &s) {
  sf::RenderStates states;
  states.blendMode = sf::BlendNone; 
  texture_.draw(s,states);
}


  sf::Vector2i PixelLayer::getOffset() const{
  return sf::Vector2i(static_cast<int>(offset_.getPosition().x),static_cast<int>(offset_.getPosition().y));
}

void PixelLayer::erase(sf::Drawable &s) {
  texture_.draw(s, sf::BlendNone);
}

void PixelLayer::shift(sf::Vector2i v){
  offset_.move(sf::Vector2f(v));
} 


void PixelLayer::drawLayer(sf::RenderTarget& target) {
  if (!masked_) {
    target.draw(offset_);
  }
}


void PixelLayer::display() {
    texture_.display();
}
