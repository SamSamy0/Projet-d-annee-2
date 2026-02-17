#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "tool.hpp"




struct parameters{

};

class ToolBar {

  std::vector<std::shared_ptr<Tool>> tools_;
  typeOutil selected_ = PIXELBRUSH;
  public:
    ToolBar(std::shared_ptr<Map> map);
    void selectTool(typeOutil outil);
    std::shared_ptr<Tool> getSelectedTool() const;
    typeOutil getSelected() const;
};
