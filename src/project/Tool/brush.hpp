#pragma once
#include "tool.hpp"
#include "../../client/clientnetwork.hpp"

class Brush : public Tool {
protected:
  sf::Vector2f size_m_ = sf::Vector2f(1, 1);
  int distance_ = 0;
  float spacing_;

  // TODO: faire ici la méthode paintSender
public:
  Brush(std::shared_ptr<Map>, ClientNetworkManager &);
  virtual void setSize(float x, float y = 1);// WARNING: la valeur par défaut je suis pas sur
  sf::Vector2f getSize();
  void onPress(sf::Vector2i pos);
  void onDrag(sf::Vector2i pos);
  void onRelease();
  virtual void paintSender(sf::Vector2i pos) = 0;
  virtual void paint(sf::Vector2i pos) = 0;
  virtual ~Brush() = default;
};
