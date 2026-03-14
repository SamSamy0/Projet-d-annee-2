#include "shift.hpp"
#include "../Layer/layer.hpp"
#include "../map.hpp"
#include "tool.hpp"
#include <memory>

Shift::Shift(std::shared_ptr<Map> map) : Tool(map) {}

void Shift::onPress(sf::Vector2i pos) {
  isDrawing_ = true;
  lastPos_ = pos;
}

void Shift::onDrag(sf::Vector2i pos) {
  if (!isDrawing_)
    return;

  sf::Vector2i delta = pos - lastPos_;
  if (delta.x != 0 || delta.y != 0) {
    map_->getCurrentLayer()->shift(delta);
  }
  lastPos_ = pos;
}

void Shift::onRelease() { isDrawing_ = false; }

Shift::~Shift() {}
