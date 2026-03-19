#pragma once
#include "Tool/tool.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class ToolBar {

  std::vector<std::shared_ptr<Tool>> tools_;
  ToolType selected_;

public:
  ToolBar(std::shared_ptr<Map> map);
  void selectTool(ToolType tool);
  std::shared_ptr<Tool> getSelectedTool() const;
  ToolType getSelected() const;
};
