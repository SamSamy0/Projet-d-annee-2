#pragma once
#include <SFML/Graphics.hpp>

enum LayerType { PIXELLAYER, SPRITELAYER };
/*
struct Lock {
  std::string author;
  bool isLocked;
};*/

class Layer {
protected:
  std::string name_;
  uint id_;
  sf::Vector2u size_;
  bool masked_ = false;
  LayerType type_;
  //std::unique_ptr<Lock> lock_ = nullptr;

public:
  Layer(uint id, std::string name, sf::Vector2u size);
  std::string getName() const;
  void setName(const std::string& name);
  uint getId()const;
  LayerType getType();
  bool isMasked() const;
  void setMasked(bool masked);
  virtual sf::Vector2i getOffset() const = 0;
  virtual void shift(sf::Vector2i v) = 0;
  virtual void drawLayer(sf::RenderTarget &target) = 0;
  virtual ~Layer() = default;
};
