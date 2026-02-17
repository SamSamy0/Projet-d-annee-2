#include "toolbar.hpp"
#include "map.hpp"
#include "layer.hpp"


ToolBar::ToolBar(std::shared_ptr<Map> map){

  tools_.push_back(std::make_shared<PixelBrush>(map));
  tools_.push_back(std::make_shared<PixelShift>(map));
}



  void ToolBar::selectTool(toolType outil){

  typeCouche type = getSelectedTool()->getMap()->getCurrentLayer()->getType();

  if((outil<=1 && type == PIXELLAYER) || (outil > 1 && type == SPRITELAYER))
  selected_ = outil;


}
  std::shared_ptr<Tool> ToolBar::getSelectedTool() const {return tools_[selected_];}
  toolType ToolBar::getSelected() const {return selected_;}
