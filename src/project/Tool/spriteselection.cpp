#include "spriteselection.hpp"
#include <memory>


 SpriteSelection::SpriteSelection(std::shared_ptr<Map> map) : Tool(map){}



bool SpriteSelection::isSelected(uint64_t id)const{
  if(std::find(selected_.begin(),selected_.end(),id) != selected_.end()){return true;}
  else{return false;}
}

void SpriteSelection::clearSelection(){selected_.clear();}

void SpriteSelection::onPress(sf::Vector2i pos){

}
