#include "toolbar.hpp"
#include "Layer/layer.hpp"
#include "Tool/pixelbrush.hpp"
#include "Tool/pixelshift.hpp"
#include "Tool/spritebrush.hpp"
#include "Tool/spriteshift.hpp"
#include "Tool/spriteeraser.hpp"
#include "Tool/nonetool.hpp"
#include "Tool/spriteselection.hpp"
#include "Tool/autofill.hpp"
#include "map.hpp"
#include <memory>

ToolBar::ToolBar(std::shared_ptr<Map> map, ClientNetworkManager &manager): selected_{NONETOOL} {
  tools_.push_back(std::make_shared<NoneTool>(map, manager));
  tools_.push_back(std::make_shared<PixelBrush>(map, manager));
  tools_.push_back(std::make_shared<PixelShift>(map, manager));
  tools_.push_back(std::make_shared<SpriteBrush>(map, manager));
  tools_.push_back(std::make_shared<SpriteEraser>(map, manager));
  tools_.push_back(std::make_shared<SpriteShift>(map, manager));
  tools_.push_back(std::make_shared<SpriteSelection>(map,manager));
  tools_.push_back(std::make_shared<AutoFill>(map,manager));
}

void ToolBar::selectTool(ToolType outil) {
  LayerType type = getSelectedTool()->getMap()->getCurrentLayer()->getType();

  if ((outil > 0 && outil <= 2 && type == PIXELLAYER) || (outil > 2 && type == SPRITELAYER) || outil == AUTOFILL){
    selected_ = outil;
  }
}
std::shared_ptr<Tool> ToolBar::getSelectedTool() const {
  return tools_[selected_];
}
ToolType ToolBar::getSelected() const { return selected_; }
