#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "Tool/tool.hpp"




// struct parameters{
//
// };

class ToolBar {

  std::vector<std::shared_ptr<Tool>> tools_;
  ToolType selected_;
  public:
    ToolBar(std::shared_ptr<Map> map, ClientNetworkManager &manager);
    void selectTool(ToolType tool);
    std::shared_ptr<Tool> getSelectedTool() const;
    ToolType getSelected() const;
};
