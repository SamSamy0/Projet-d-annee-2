#include "spriteeraser.hpp"
#include "../Layer/layer.hpp"
#include "../Layer/spritelayer.hpp"
#include "../map.hpp"
#include <memory>





SpriteEraser::SpriteEraser(std::shared_ptr<Map> map): Brush(map){}

void SpriteEraser::setShape(Shape s){shape_ = s;}

  void SpriteEraser::paint(sf::Vector2i pos){
  std::shared_ptr<Layer> layer = map_->getCurrentLayer();
  if(layer->getType() != SPRITELAYER){return;}
  std::shared_ptr<SpriteLayer> spritelayer = static_pointer_cast<SpriteLayer>(layer);
  if(!spritelayer){return;}




  

  

}
