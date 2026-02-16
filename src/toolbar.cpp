#include "toolbar.hpp"
#include "map.hpp"


ToolBar::ToolBar(std::shared_ptr<Map> map){

  tools_.push_back(std::make_shared<PixelBrush>(map));
  tools_.push_back(std::make_shared<PixelShift>(map));
}



  void ToolBar::selectTool(typeOutil outil){selected_ = outil;}
  std::shared_ptr<Tool> ToolBar::getSelectedTool() const {return tools_[selected_];}
  typeOutil ToolBar::getSelected() const {return selected_;}
