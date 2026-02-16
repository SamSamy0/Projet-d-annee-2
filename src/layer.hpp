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
  virtual void drawLayer(sf::RenderWindow& window) = 0;
  virtual void draw(sf::Drawable &s) = 0;
  virtual ~Layer() = default;
};

class PixelLayer : public Layer {

  sf::RenderTexture texture_;
  sf::Sprite offset_;

public:
  PixelLayer(std::string name, sf::Vector2u size);

  sf::RenderTexture &getTexture();
  void drawLayer(sf::RenderWindow& window) override;
  void draw(sf::Drawable &s) override;
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
