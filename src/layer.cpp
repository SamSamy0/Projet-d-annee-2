#include "layer.hpp"

sf::RenderTexture& PixelLayer::getTexture(){return texture_;}
void PixelLayer::draw(sf::Drawable& s){texture_.draw(s);texture_.display();}
void PixelLayer::errase(sf::Drawable& s){texture_.draw(s,sf::BlendNone);texture_.display();}

