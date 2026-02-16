#include "layer.hpp"
#include "map.hpp"
#include <memory>

Layer::Layer(std::string name, sf::Vector2u size) : name_{name}, size_{size} {}

PixelLayer::PixelLayer(std::string name,sf::Vector2u size) : Layer(name, size), offset_(texture_.getTexture()) {
  if(!texture_.resize(size)){
    // NOTE:GERER L'ERREUR 
    }
    offset_.setTexture(texture_.getTexture());
    texture_.clear(sf::Color::Transparent);
    texture_.display();
}
sf::RenderTexture &PixelLayer::getTexture() { return texture_; }
void PixelLayer::draw(sf::Drawable &s) {
  texture_.draw(s);
  texture_.display();
}
void PixelLayer::errase(sf::Drawable &s) {
  texture_.draw(s, sf::BlendNone);
  texture_.display();
}

void PixelLayer::shift(sf::Vector2i v){
  offset_.move(sf::Vector2f(v));
} 

void PixelLayer::display(){texture_.display();}

void PixelLayer::drawLayer(sf::RenderWindow& window) {
  if (!Masked_) {
    sf::Sprite layer{texture_.getTexture()};
    window.draw(layer);
  }
}
