#pragma once
#include <SFML/Graphics.hpp>

enum LayerType { PIXELLAYER, SPRITELAYER };

class Layer {
protected:
  std::string name_;
  sf::Vector2u size_;
  bool masked_ = false;
  LayerType type_;

public:
  Layer(std::string name, sf::Vector2u size);
  LayerType getType();
  virtual sf::Vector2i getOffset() const = 0;
  virtual void shift(sf::Vector2i v) = 0;
  virtual void drawLayer(sf::RenderTarget &target) = 0;
  virtual ~Layer() = default;
};
