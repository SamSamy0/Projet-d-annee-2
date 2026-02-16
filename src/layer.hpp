#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class User;
class Map;

class Layer {
protected:
  sf::Vector2u size_;
  std::string name_;
  bool locked_ = false;
  // User lock_;
  bool Masked_ = false;

public:
  Layer(std::string name, sf::Vector2u size);
  virtual void draw(sf::Drawable &s) = 0;
  virtual ~Layer() = default;
};

class PixelLayer : public Layer {

  sf::RenderTexture texture_;
  sf::Sprite offset_;

public:
  PixelLayer(std::string name, sf::Vector2u size);

  sf::RenderTexture &getTexture();
  void draw(sf::Drawable &s);
  void errase(sf::Drawable &s);
  void shift(sf::Vector2i v);
  void display();
};
class SpriteLayer : public Layer {
  std::vector<sf::Sprite> sprites_;

public:
  SpriteLayer(); // TODO: à définir
  void draw(sf::Drawable &s);
};
