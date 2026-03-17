#pragma once
#include <SFML/Graphics.hpp>

enum LayerType { PIXELLAYER, SPRITELAYER };

class Layer {
protected:
  uint64_t id_;
  std::string name_;
  uint id_;
  sf::Vector2u size_;
  bool masked_ = false;
  LayerType type_;

public:
  Layer(uint64_t id, std::string name, sf::Vector2u size);
  std::string getName() const;
  uint getId()const;
  LayerType getType();
  bool isMasked() const;
  void setMasked(bool masked);
  virtual sf::Vector2i getOffset() const = 0;
  virtual void shift(sf::Vector2i v) = 0;
  virtual void drawLayer(sf::RenderTarget &target) = 0;
  virtual ~Layer() = default;
};
