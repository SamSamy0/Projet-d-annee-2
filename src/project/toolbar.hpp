#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "Tool/tool.hpp"




struct parameters{

};

class ToolBar {

  std::vector<std::shared_ptr<Tool>> tools_;
  toolType selected_;
  public:
    ToolBar(std::shared_ptr<Map> map);
    void selectTool(toolType outil);
    std::shared_ptr<Tool> getSelectedTool() const;
    toolType getSelected() const;
};
