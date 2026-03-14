#include "toolbar.hpp"
#include "Layer/layer.hpp"
#include "Tool/pixelbrush.hpp"
#include "Tool/pixelshift.hpp"
#include "Tool/spriteshift.hpp"
#include "Tool/spriteeraser.hpp"
#include "Tool/nonetool.hpp"
#include "map.hpp"

ToolBar::ToolBar(std::shared_ptr<Map> map): selected_{NONETOOL} {
  tools_.push_back(std::make_shared<NoneTool>(map));
  tools_.push_back(std::make_shared<PixelBrush>(map));
  tools_.push_back(std::make_shared<PixelShift>(map));
  tools_.push_back(std::make_shared<PixelShift>(map));
  tools_.push_back(std::make_shared<SpriteEraser>(map));
  tools_.push_back(std::make_shared<SpriteShift>(map));
}

void ToolBar::selectTool(ToolType outil) {
  LayerType type = getSelectedTool()->getMap()->getCurrentLayer()->getType();

  if ((outil > 0 && outil <= 1 && type == PIXELLAYER) || (outil > 0 && outil > 1 && type == SPRITELAYER)){
    selected_ = outil;
  }
}
std::shared_ptr<Tool> ToolBar::getSelectedTool() const {
  return tools_[selected_];
}
ToolType ToolBar::getSelected() const { return selected_; }
