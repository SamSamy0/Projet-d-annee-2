#pragma once
#include <SFML/Graphics.hpp>
#include <memory>


enum typeCouche{PIXELLAYER,
SPRITELAYER};

using LayerContent = std::variant<sf::RenderTexture*, std::vector<sf::Sprite>*>;

class Layer {
protected:
  std::string name_;
  sf::Vector2u size_;
  bool Masked_ = false;
  typeCouche type_;

public:
  Layer(std::string name, sf::Vector2u size);
  typeCouche getType();
  virtual LayerContent getLayerContent() = 0;
  virtual void drawLayer(sf::RenderWindow& window) = 0;
  virtual void draw(sf::Drawable &s) = 0;
  virtual void errase(sf::Drawable &s) = 0;
  virtual void shift(sf::Vector2i v) = 0;
  virtual void display() = 0;
  virtual ~Layer() = default;
};
