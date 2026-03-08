#include "spriteeraser.hpp"
#include "../Layer/layer.hpp"
#include "../map.hpp"
#include <memory>





SpriteEraser::SpriteEraser(std::shared_ptr<Map> map): Brush(map){}

  void SpriteEraser::paint(sf::Vector2i pos){
  std::shared_ptr<Layer> layer = map_->getCurrentLayer();
  if(layer->getType() != SPRITELAYER){return;}
  

  

}
