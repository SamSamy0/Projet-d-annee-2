#include "layer.hpp"



Layer::Layer(std::string name) : name_{name}{}


PixelLayer::PixelLayer(std::string name) : Layer(name){
  if(!texture_.resize(sf::Vector2u(map_->getWidth(),map_->getHeight())){
    //NOTE:GERER L'ERREUR 
    }
    offset_.setTexture(texture_.getTexture());
    texture_.clear(sf::Color::Transparent);
    texture_.display();
}
sf::RenderTexture& PixelLayer::getTexture(){return texture_;}
void PixelLayer::draw(sf::Drawable& s){texture_.draw(s);texture_.display();}
void PixelLayer::errase(sf::Drawable& s){texture_.draw(s,sf::BlendNone);texture_.display();}

