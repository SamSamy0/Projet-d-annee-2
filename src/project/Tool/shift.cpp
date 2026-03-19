#include "shift.hpp"
#include "../Layer/layer.hpp"
#include "../map.hpp"
#include "../../client/clientnetwork.hpp"
#include "tool.hpp"
#include <memory>

Shift::Shift(std::shared_ptr<Map> map, ClientNetworkManager &manager) : Tool(map, manager) {}

void Shift::onPress(sf::Vector2i pos) {
  if(!map_->getCurrentLayer()->isMasked()){
    isDrawing_ = true;
    lastPos_ = pos;
  }
}

void Shift::onDrag(sf::Vector2i pos) {
  if (!isDrawing_)
    return;

  sf::Vector2i delta = pos - lastPos_;
  if (delta.x != 0 || delta.y != 0) {
    shiftSender(delta);
  }
  lastPos_ = pos;
}

void Shift::shift(sf::Vector2i delta){
  map_->getCurrentLayer()->shift(delta);
}

void Shift::shiftSender(sf::Vector2i delta){
  shift(delta);
  manager_.shiftLayer(map_->getId(),map_->getCurrentLayer()->getId(),delta.x,delta.y);

}

void Shift::onRelease() { isDrawing_ = false; }

Shift::~Shift() {}

