#include "project.hpp"
#include "map.hpp"
#include "layer.hpp"
#include "tool.hpp"
#include <memory>



Project::Project(unsigned int scale, sf::Vector2u size): map_{std::make_shared<Map>(1,size,scale)},
  toolbar_{map_}{}

unsigned int Project::getScale() { return map_->getScale(); }
std::shared_ptr<Map> Project::getMap(){return map_;}
ToolBar Project::getToolBar(){return toolbar_;}

