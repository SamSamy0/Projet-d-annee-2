#pragma once
#include <SFML/Graphics.hpp>
#include <memory>


enum LayerType{PIXELLAYER,
SPRITELAYER};

using LayerContent = std::variant<sf::RenderTexture*, std::vector<sf::Sprite>*>;

class Layer {
protected:
  std::string name_;
  sf::Vector2u size_;
  bool masked_ = false;
  LayerType type_;

public:
  Layer(std::string name, sf::Vector2u size);
  LayerType getType();
  virtual LayerContent getLayerContent() = 0;
  virtual void drawLayer(sf::RenderTarget& target) = 0;
  virtual void draw(sf::Drawable &s) = 0;
  virtual void errase(sf::Drawable &s) = 0;
  virtual void shift(sf::Vector2i v) = 0;
  virtual void display() = 0;
  virtual ~Layer() = default;
};
