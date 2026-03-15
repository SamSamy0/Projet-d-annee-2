#pragma once
#include "brush.hpp"




class SpriteBrush : public Brush{
  std::vector<sf::Sprite> sprites_;




public:
  void paint(sf::Vector2i pos);




};
