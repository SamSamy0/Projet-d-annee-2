#include "pixellayer.hpp"

PixelLayer::PixelLayer(std::string name,sf::Vector2u size) : Layer(name, size), offset_(texture_.getTexture()) {
  if(!texture_.resize(size)){
    // NOTE:GERER L'ERREUR 
    }
    offset_.setTexture(texture_.getTexture());
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
void PixelLayer::errase(sf::Drawable &s) {
  texture_.draw(s, sf::BlendNone);
}

void PixelLayer::shift(sf::Vector2i v){
  offset_.move(sf::Vector2f(v));
} 


void PixelLayer::drawLayer(sf::RenderWindow& window) {
  if (!Masked_) {
    sf::Sprite layer{texture_.getTexture()};
    window.draw(layer);
  }
}


void PixelLayer::display() {
    sf::Vector2f mouvement = offset_.getPosition();

    if (mouvement.x == 0.0f && mouvement.y == 0.0f) {
        return; 
    }

    sf::Texture currentTexture = texture_.getTexture();
    sf::Sprite tempSprite(currentTexture);
    tempSprite.setPosition(sf::Vector2f(mouvement.x, mouvement.y));

    texture_.clear(sf::Color::Transparent);
    texture_.draw(tempSprite);
    texture_.display();

    offset_.setPosition(sf::Vector2f(0, 0));
}
