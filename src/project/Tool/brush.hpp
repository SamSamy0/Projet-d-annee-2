#pragma once
#include "tool.hpp"

class Brush : public Tool {
protected:
  // TODO :CHANGER EN FLOAT ET VERIFIER QUE C'EST BIEN ARRONDI A CHAQUE FOIS
  // QU'ON APPLIQUE LA SCALE
  sf::Vector2f size_m_ = sf::Vector2f(1, 1);
  int distance_ = 0.0f;
  int spacing_;

public:
  Brush(std::shared_ptr<Map>);
  void setSize(float x, float y);
  sf::Vector2f getSize();
  void onPress(sf::Vector2i pos);
  void onDrag(sf::Vector2i pos);
  void onRelease();
  virtual void paint(sf::Vector2i pos) = 0;
  virtual ~Brush() = default;
};
