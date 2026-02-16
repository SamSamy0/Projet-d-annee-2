#include "project.hpp"
#include "map.hpp"
#include "layer.hpp"
#include "tool.hpp"
#include <memory>



Project::Project(unsigned int scale, sf::Vector2u size){

  map_ = std::make_shared<Map>(1,size,scale);

  tools_.push_back(std::make_shared<PixelBrush>(map_));
  tools_.push_back(std::make_shared<PixelShift>(map_));

}
unsigned int Project::getScale() { return map_->getScale(); }
std::shared_ptr<Map> Project::getMap(){return map_;}
