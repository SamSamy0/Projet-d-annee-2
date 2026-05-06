#include "receiverwindow.hpp"
#include "../Window/MenuView.hpp"
#include "../Window/projectWidgets/GameView.hpp"
#include "../project/Chat/chat.hpp"
#include "../project/Chat/date.hpp"
#include "../project/Chat/userMessage.hpp"
#include "../project/Chat/systemNotification.hpp"
#include "../project/Layer/spritelayer.hpp"
#include "../project/Tool/pixelbrush.hpp"
#include "../project/Tool/pixelshift.hpp"
#include "../project/Tool/spritebrush.hpp"
#include "../project/Tool/spriteeraser.hpp"
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
  } else {
    app_->showLoginError("Identifiant ou mot de passe incorrect.");
  }
}

void ReceiverInWindow::addProjectToList(ProjectData projet) {
  app_->addProjectList(projet);
}

void ReceiverInWindow::updateProjectNameInList(uint id,
                                               const std::string &newName) {
  app_->updateProjectNameInList(id, newName);
}

void ReceiverInWindow::clearProjList() { app_->clearProjList(); }
void ReceiverInWindow::clearMemberList() {
  dynamic_cast<GameView *>(app_->getCurrentView().get())->clearMemberList();
}

void ReceiverInWindow::setUserId(uint newId) { app_->setUserId(newId); }

void ReceiverInWindow::updateCreatedProjectId(uint projId) {
  app_->updateCreatedProjectId(projId);
}

void ReceiverInWindow::setState() {
  app_->changeView(std::make_unique<GameView>(*app_));
}

void ReceiverInWindow::updateShareToken(std::string token) {
  app_->updateShareToken(token);
}
void ReceiverInWindow::addProjectData(unsigned int scale, sf::Vector2u size,
                                      std::string name, uint id) {
  app_->loadProjectData(scale, size, name, id);
}

void ReceiverInWindow::addProjectData(unsigned int scale, sf::Vector2u size,
                                      std::string name, uint id,
                                      uint nextLayerId,
                                      const std::vector<LayerLoadData> &layers,
                                      Chat chat, const std::map<uint, sf::Texture> &textureMap) {
  app_->loadProjectData(scale, size, name, id, nextLayerId, layers, chat, textureMap);
}

void ReceiverInWindow::createLayer(LayerType type) {
  dynamic_cast<GameView *>(app_->getCurrentView().get())->createLayer(type);
}

void ReceiverInWindow::deleteLayer(uint current_layer_id) {
  dynamic_cast<GameView *>(app_->getCurrentView().get())
      ->deleteLayer(current_layer_id);
}

void ReceiverInWindow::renameLayer(uint layer_id, std::string name) {
  dynamic_cast<GameView *>(app_->getCurrentView().get())
      ->renameLayer(layer_id, name);
}

void ReceiverInWindow::layerUp(uint layer_id) {
  dynamic_cast<GameView *>(app_->getCurrentView().get())->layerUp(layer_id);
}

void ReceiverInWindow::layerDown(uint layer_id) {
  dynamic_cast<GameView *>(app_->getCurrentView().get())->layerDown(layer_id);
}

void ReceiverInWindow::drawPixelBrush(uint layer_id, int pos_x, int pos_y,
                                      uint8_t r, uint8_t g, uint8_t b,
                                      uint8_t a, Shape shape, bool eraser,
                                      float size_x, float size_y) {
  ToolBar &toolbar = app_->getProject()->getToolBar();
  std::shared_ptr<Map> map = app_->getProject()->getMap();
  ToolType last_tool = toolbar.getSelected();
  uint last_layer_id = map->getCurrentLayer()->getId();
  map->selectLayerId(layer_id);
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
  pixelbrush->paint(sf::Vector2i(pos_x, pos_y));

  pixelbrush->setShape(last_shape);
  pixelbrush->setSize(last_size.x, last_size.y);
  pixelbrush->setEraser(last_is_eraser);
  pixelbrush->setColor(last_color);
  map->selectLayerId(last_layer_id);
  toolbar.selectTool(last_tool);
}

void ReceiverInWindow::drawSprite(uint layer_id, std::string asset_id,
                                  int pos_x, int pos_y, float size) {
  ToolBar &toolbar = app_->getProject()->getToolBar();
  std::shared_ptr<Map> map = app_->getProject()->getMap();
  ToolType last_tool = toolbar.getSelected();
  uint last_layer_id = map->getCurrentLayer()->getId();
  map->selectLayerId(layer_id);
  toolbar.selectTool(SPRITEBRUSH);
  std::shared_ptr<SpriteBrush> spritebrush =
      static_pointer_cast<SpriteBrush>(toolbar.getSelectedTool());
  sf::Vector2f last_size = spritebrush->getSize();
  spritebrush->setSize(size, 0);

  spritebrush->paint(sf::Vector2i(pos_x, pos_y),
                     map->getAssetManager().getAsset(asset_id));

  spritebrush->setSize(last_size.x, last_size.y);
  map->selectLayerId(last_layer_id);
  toolbar.selectTool(last_tool);
}

void ReceiverInWindow::shiftLayer(uint layer_id, int delta_x, int delta_y) {
  ToolBar &toolbar = app_->getProject()->getToolBar();
  std::shared_ptr<Map> map = app_->getProject()->getMap();
  ToolType last_tool = toolbar.getSelected();
  uint last_layer_id = map->getCurrentLayer()->getId();
  map->selectLayerId(layer_id);
  if (map->getCurrentLayer()->getType() == SPRITELAYER) {
    toolbar.selectTool(SPRITESHIFT);
    std::shared_ptr<SpriteShift> shift =
        static_pointer_cast<SpriteShift>(toolbar.getSelectedTool());
    shift->shift(sf::Vector2i(delta_x, delta_y));
  } else {
    toolbar.selectTool(PIXELSHIFT);
    std::shared_ptr<PixelShift> shift =
        static_pointer_cast<PixelShift>(toolbar.getSelectedTool());
    shift->shift(sf::Vector2i(delta_x, delta_y));
  }

  map->selectLayerId(last_layer_id);
  toolbar.selectTool(last_tool);
}

void ReceiverInWindow::eraseSprite(uint layer_id, uint sprite_id) {
  shared_ptr<Layer> layer = app_->getProject()->getMap()->getLayer(layer_id);
  if (layer->getType() == SPRITELAYER)
    static_pointer_cast<SpriteLayer>(layer)->erase(sprite_id);
}

void ReceiverInWindow::moveSprite(uint layer_id, uint sprite_id,
                                  sf::Vector2i v) {
  static_pointer_cast<SpriteLayer>(
      app_->getProject()->getMap()->getLayer(layer_id))
      ->shiftSprite(sprite_id, v);
}
void ReceiverInWindow::resizeSprite(uint layer_id, uint sprite_id,
                                    sf::Vector2f pos, float scale) {
  static_pointer_cast<SpriteLayer>(
      app_->getProject()->getMap()->getLayer(layer_id))
      ->resizeSprite(sprite_id, pos, scale);
}
void ReceiverInWindow::rotateSprite(uint layer_id, uint sprite_id, float angle,
                                    sf::Vector2f pos) {
  static_pointer_cast<SpriteLayer>(
      app_->getProject()->getMap()->getLayer(layer_id))
      ->rotateSprite(sprite_id, angle, pos);
}



void ReceiverInWindow::autoFill(uint layer_id,std::vector<std::string> asset_id, std::vector<sf::Vector2f> pos, float rotatation, float size){
  dynamic_cast<GameView *>(app_->getCurrentView().get())->autoFill(layer_id,asset_id,pos,rotatation,size);
}


void ReceiverInWindow::setMemberList(std::vector<MemberEntry> memberList) {
  if (auto gameView = dynamic_cast<GameView *>(app_->getCurrentView().get())) {
    gameView->setAllUsers(memberList);
  }
}

void ReceiverInWindow::updateMemberList(uint projectId, uint target,
                                        int8_t role) {
  // If we are in a game view
  if (auto gameView = dynamic_cast<GameView *>(app_->getCurrentView().get())) {
    // If its about the project we are in
    if (app_->getProject() and app_->getProject()->getId() == projectId) {
      gameView->updateMemberRole(target, role);
    }
  }
}
void ReceiverInWindow::kickUser(uint targetId) {
  if (app_->getUser().getId() == targetId) {
    if (auto gameView =
            dynamic_cast<GameView *>(app_->getCurrentView().get())) {
      gameView->popupWarning("kick");
    } else {
      app_->getNetwork().getProjectList();
    }
  } else {
    app_->getNetwork().getUsersProjects(app_->getProject()->getId());
  }
}

void ReceiverInWindow::addChatMess(std::string pseudo, std::string message,
                                   int min, int hour, int day, int month,
                                   int year) {
  Date date(min, hour, day, month, year);
  if (auto gameView = dynamic_cast<GameView *>(app_->getCurrentView().get())) {
    if (app_->getProject()) {
      app_->getProject()->getChat().addMessage(
          make_shared<UserMessage>(User(pseudo, 0), date, message));
    }
    gameView->refreshChat();
  }
}

void ReceiverInWindow::addChatSyst(std::string pseudo, uint8_t type, int min,
                                   int hour, int day, int month, int year) {
  Date date(min, hour, day, month, year);
  if (auto gameView = dynamic_cast<GameView *>(app_->getCurrentView().get())) {
    if (app_->getProject()) {
      app_->getProject()->getChat().addMessage(make_shared<SystemNotification>(
          User(pseudo, 0), date, static_cast<typeNotification>(type)));
    }
    gameView->refreshChat();
  }
}

void ReceiverInWindow::addSprite(uint spriteId, sf::Texture sprite){
  app_->getProject()->getMap()->getAssetManager().addAsset(spriteId, sprite);
  app_->refreshImportPanel();
}