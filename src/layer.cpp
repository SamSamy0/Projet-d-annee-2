#include "layer.hpp"
#include "map.hpp"
#include <memory>

Layer::Layer(std::string name, std::shared_ptr<Map> map) : name_{name}, map_{map} {}

PixelLayer::PixelLayer(std::string name,std::shared_ptr<Map> map) : Layer(name, map), offset_(texture_.getTexture()) {
  if(!texture_.resize(sf::Vector2u(map_->getSize().x,map_->getSize().y))){
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

void PixelLayer::shift(sf::Vector2u v){} //TODO: à défini
