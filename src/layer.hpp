#pragma once 
#include <SFML/Graphics.hpp>


class User;

class Layer{
protected:
  std::string name_;
  bool locked_;
  // User lock_;
  bool Masked_;

public:
  virtual void draw(sf::Drawable& s) = 0;
  virtual ~Layer() = default;

};

class PixelLayer : public Layer{

  sf::RenderTexture texture_;
  sf::Sprite offset_;

public:
  sf::RenderTexture& getTexture();
  void draw(sf::Drawable& s);
  void errase(sf::Drawable& s);


};
class SpriteLayer : public Layer{


public:
  void draw(sf::Drawable& s);

};
