#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class User;
class Map;

class Layer {
protected:
  std::shared_ptr<Map> map_;
  std::string name_;
  bool locked_ = false;
  // User lock_;
  bool Masked_ = false;

public:
  Layer(std::string name);
  virtual void draw(sf::Drawable &s) = 0;
  virtual ~Layer() = default;
};

class PixelLayer : public Layer {

  sf::RenderTexture texture_;
  sf::Sprite offset_;

public:
  PixelLayer(std::string name); // TODO: à définir

  sf::RenderTexture &getTexture();
  void draw(sf::Drawable &s);
  void errase(sf::Drawable &s);
};
class SpriteLayer : public Layer {

public:
  SpriteLayer(); // TODO: à définir
  void draw(sf::Drawable &s);
};
