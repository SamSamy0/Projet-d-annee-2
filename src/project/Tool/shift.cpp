#include "shift.hpp"
#include "tool.hpp"
#include "../map.hpp"
#include <memory>
#include "../Layer/layer.hpp"


Shift::Shift(std::shared_ptr<Map> map):Tool(map){}

void Shift::onPress(sf::Vector2i pos){
  isDrawing_ = true;
  lastPos_ = pos;
}

void Shift::onDrag(sf::Vector2i pos){
  if(!isDrawing_)
    return;

  sf::Vector2i delta = pos-lastPos_;

  if (delta.x != 0 || delta.y != 0){
    map_->getCurrentLayer()->shift(delta);
  }

}

void Shift::onRelease(){
  isDrawing_ = false;
  lastPos_ = sf::Vector2i(0,0);
}


Shift::~Shift(){}
