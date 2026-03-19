#include "receiverwindow.hpp"
#include "../Window/MenuView.hpp"
#include "../Window/projectWidgets/GameView.hpp"
#include "../project/Tool/pixelbrush.hpp"
#include "../project/Tool/pixelshift.hpp"
#include "../project/Tool/spriteshift.hpp"
#include "../project/Tool/tool.hpp"
#include "../project/project.hpp"
#include "../project/toolbar.hpp"
#include <iostream>
#include <memory>

ReceiverInWindow::ReceiverInWindow(Application &app) : app_(&app) {}

void ReceiverInWindow::switchConnectState(uint8_t connect) {
  if (connect == 1) {
    app_->changeView(std::make_unique<MenuView>(*app_));
    app_->getNetwork().getProjectList();
  }
}

void ReceiverInWindow::addProjectToList(ProjectData projet) {
  app_->addProjectList(projet);
}

void ReceiverInWindow::updateProjectNameInList(uint id, const std::string& newName){
    app_->updateProjectNameInList(id, newName);
    
}

void ReceiverInWindow::updateCreatedProjectId(uint projId) {
    app_->updateCreatedProjectId(projId);
}

void ReceiverInWindow::setState() {
  app_->changeView(std::make_unique<GameView>(*app_));
}

void ReceiverInWindow::updateShareToken(std::string token){
    app_->updateShareToken(token);
}
void ReceiverInWindow::addProjectData(unsigned int scale, sf::Vector2u size,
                                      std::string name, uint id) {
  app_->loadProjectData(scale, size, name, id);
}

void ReceiverInWindow::drawPixelBrush(uint layer_id, int pos_x, int pos_y,
                                      uint8_t r, uint8_t g, uint8_t b,
                                      uint8_t a, Shape shape, bool eraser,
                                      float size_x, float size_y) {

  ToolBar &toolbar = app_->getProject()->getToolBar();
  std::shared_ptr<Map> map = app_->getProject()->getMap();
  ToolType last_tool = toolbar.getSelected();
  uint last_layer_id = map->getCurrentLayer()->getId();
  toolbar.selectTool(PIXELBRUSH);
  std::shared_ptr<PixelBrush> pixelbrush =
      static_pointer_cast<PixelBrush>(toolbar.getSelectedTool());
  Shape last_shape = pixelbrush->getShape();
  sf::Vector2f last_size = pixelbrush->getSize();
  bool last_is_eraser = pixelbrush->getEraser();
  sf::Color last_color = pixelbrush->getColor();

  pixelbrush->setShape(shape);
  if (shape != DIAMOND) {
    pixelbrush->setSize(size_x, 1);
  } else {
    pixelbrush->setSize(size_x, size_y);
  }
  pixelbrush->setEraser(eraser);
  if (!eraser) {
    pixelbrush->setColor(sf::Color(r, g, b, a));
  }
  map->selectLayerId(layer_id);
  pixelbrush->paint(sf::Vector2i(pos_x, pos_y));
  map->selectLayerId(last_layer_id);

  pixelbrush->setShape(last_shape);
  pixelbrush->setSize(last_size.x, last_size.y);
  pixelbrush->setEraser(last_is_eraser);
  pixelbrush->setColor(last_color);
  toolbar.selectTool(last_tool);
}



void ReceiverInWindow::shiftLayer(uint layer_id, int delta_x, int delta_y){
  ToolBar &toolbar = app_->getProject()->getToolBar();
  std::shared_ptr<Map> map = app_->getProject()->getMap();
  ToolType last_tool = toolbar.getSelected();
  uint last_layer_id = map->getCurrentLayer()->getId();
  map->selectLayerId(layer_id);
  if(map->getCurrentLayer()->getType() == SPRITELAYER){
    toolbar.selectTool(SPRITESHIFT);
    std::shared_ptr<SpriteShift> shift =
        static_pointer_cast<SpriteShift>(toolbar.getSelectedTool());
    shift->shift(sf::Vector2i(delta_x,delta_y));
  }
  else{toolbar.selectTool(PIXELSHIFT);
    std::shared_ptr<PixelShift> shift =
        static_pointer_cast<PixelShift>(toolbar.getSelectedTool());
    shift->shift(sf::Vector2i(delta_x,delta_y));
  }

  map->selectLayerId(last_layer_id);
  toolbar.selectTool(last_tool);

}

