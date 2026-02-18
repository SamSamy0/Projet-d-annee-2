#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class User;
class Map;

enum typeCouche{PIXELLAYER,
SPRITELAYER};

class Layer {
protected:
  std::string name_;
  sf::Vector2u size_;
  bool locked_ = false;
  // User lock_;
  bool Masked_ = false;
  typeCouche type_;

public:
  Layer(std::string name, sf::Vector2u size);
  typeCouche getType();
  virtual sf::RenderTexture &getTexture() {}
  virtual void drawLayer(sf::RenderWindow& window) = 0;
  virtual void draw(sf::Drawable &s) = 0;
  virtual void errase(sf::Drawable &s) = 0;
  virtual void shift(sf::Vector2i v) = 0;
  virtual void display() = 0;
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
  void errase(sf::Drawable &s) override;
  void shift(sf::Vector2i v)override;
  void display()override;
};
class SpriteLayer : public Layer {
  std::vector<sf::Sprite> sprites_;

public:
  SpriteLayer(); // TODO: à définir
  void drawLayer(sf::RenderWindow& window) override;
  void draw(sf::Drawable &s) override;
  void errase(sf::Drawable &s) override;
  void shift(sf::Vector2i v)override;
  void display()override;
};
