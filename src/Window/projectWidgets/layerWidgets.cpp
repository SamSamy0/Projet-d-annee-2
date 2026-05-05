// #include "../Window.hpp"
#include "../../project/Layer/layer.hpp"
#include "../../project/Layer/spritelayer.hpp"
#include "../../project/Tool/pixelbrush.hpp"
#include "../Application.hpp"
#include "GameView.hpp"
#include <memory>
#include <algorithm>

void GameView::initLayerPanel() {
  auto& gui = app_.getGui();

  layerPanel_ = tgui::Panel::create();
  layerPanel_->setSize("20%", "57%");
  layerPanel_->setPosition("1%", "6%");
  layerPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  layerPanel_->getRenderer()->setBorders({1});
  layerPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  layerPanel_->getRenderer()->setRoundedBorderRadius(10);
  gui.add(layerPanel_);

  auto titleLabel = tgui::Label::create("Couches");
  titleLabel->setSize("100%", "9%");
  titleLabel->setPosition("0%", "0%");
  titleLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
  titleLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
  titleLabel->getRenderer()->setTextColor(tgui::Color::White);
  layerPanel_->add(titleLabel);

  layersList_ = tgui::ScrollablePanel::create();
  layersList_->setSize("90%", "48%");
  layersList_->setPosition("5%", "9%");
  layersList_->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  layersList_->getRenderer()->setBorders({1});
  layersList_->getRenderer()->setBorderColor(tgui::Color(70, 75, 85));
  layersList_->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  layersList_->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  layerPanel_->add(layersList_);

  vector<shared_ptr<Layer>> &layers = project->getMap()->getLayers();
  unsigned int selected = project->getMap()->getLayerSelected();
  for (int i = layers.size()-1; i >= 0; i--) {
      auto layerButton = tgui::Button::create(layers[i]->getName());
      bool isSelected = (i == selected);
      layerButton->getRenderer()->setBackgroundColor(isSelected ? tgui::Color(60, 130, 200) : tgui::Color(36, 40, 47));
      layerButton->getRenderer()->setBackgroundColorHover(isSelected ? tgui::Color(75, 150, 220) : tgui::Color(50, 56, 66));
      layerButton->getRenderer()->setTextColor(tgui::Color::White);
      layerButton->getRenderer()->setBorders({1});
      layerButton->getRenderer()->setBorderColor(tgui::Color(70, 75, 85));
      layerButton->getRenderer()->setRoundedBorderRadius(6);
      layerButton->setSize("89%", "18%");
      layerButton->setPosition("5%", tgui::String(std::to_string(i * 19.76f) + "%"));
      layerButton->onClick([this, i]() {
      LayerType type = project->getMap()->getCurrentLayer()->getType();
      project->getMap()->selectLayer(static_cast<unsigned int>(i));
      checkTypeTool(type);
      refreshLayerList();
    });
    layersList_->add(layerButton);
    layerButton->setTextSize(15);
  }

  auto renameLayerButton = tgui::Button::create("Renommer");
  renameLayerButton->setSize("90%", "7%");
  renameLayerButton->setPosition("5%", "59%");
  renameLayerButton->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
  renameLayerButton->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
  renameLayerButton->getRenderer()->setTextColor(tgui::Color::White);
  renameLayerButton->getRenderer()->setBorders({0});
  renameLayerButton->getRenderer()->setRoundedBorderRadius(8);
  renameLayerButton->onClick([this]() {
    auto& gui = app_.getGui();

    auto popup = tgui::Panel::create();
    popup->setSize("22%", "18%");
    popup->setPosition("39%", "41%");
    popup->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
    popup->getRenderer()->setBorders({1});
    popup->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
    popup->getRenderer()->setRoundedBorderRadius(8);

    auto newLayerNameInput = tgui::EditBox::create();
    newLayerNameInput->setSize("82%", "33%");
    newLayerNameInput->setPosition("9%", "11%");
    auto& layers = project->getMap()->getLayers();
    unsigned int selectedLayer = project->getMap()->getLayerSelected();
    newLayerNameInput->setText(layers[selectedLayer]->getName());
    popup->add(newLayerNameInput);

    auto confirmNameButton = tgui::Button::create("OK");
    confirmNameButton->setSize("36%", "33%");
    confirmNameButton->setPosition("9%", "56%");
    confirmNameButton->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
    confirmNameButton->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
    confirmNameButton->getRenderer()->setTextColor(tgui::Color::White);
    confirmNameButton->getRenderer()->setBorders({0});
    confirmNameButton->getRenderer()->setRoundedBorderRadius(6);
    popup->add(confirmNameButton);

    auto cancelNameButton = tgui::Button::create("Annuler");
    cancelNameButton->setSize("36%", "33%");
    cancelNameButton->setPosition("55%", "56%");
    cancelNameButton->getRenderer()->setBackgroundColor(tgui::Color(90, 50, 50));
    cancelNameButton->getRenderer()->setBackgroundColorHover(tgui::Color(110, 65, 65));
    cancelNameButton->getRenderer()->setTextColor(tgui::Color::White);
    cancelNameButton->getRenderer()->setBorders({0});
    cancelNameButton->getRenderer()->setRoundedBorderRadius(6);
    popup->add(cancelNameButton);

    gui.add(popup);

    confirmNameButton->onClick([this, popup, newLayerNameInput]() {
      auto& gui = app_.getGui();
      auto& layers = project->getMap()->getLayers();
      unsigned int selectedLayer = project->getMap()->getLayerSelected();
      std::string newName = newLayerNameInput->getText().toStdString();
      if (!newName.empty()) layers[selectedLayer]->setName(newName);
      app_.getNetwork().renameLayer(project->getId(),project->getMap()->getCurrentLayer()->getId(),newName);
      refreshLayerList();
      gui.remove(popup);
    });

    cancelNameButton->onClick([this, popup]() { app_.getGui().remove(popup); });
  });
  layerPanel_->add(renameLayerButton);
  renameLayerButton->setTextSize(15);

  auto goUpLayer = tgui::Button::create("▼");
  goUpLayer->setSize("44%", "7%");
  goUpLayer->setPosition("5%", "68%");
  goUpLayer->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
  goUpLayer->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
  goUpLayer->getRenderer()->setTextColor(tgui::Color::White);
  goUpLayer->getRenderer()->setBorders({0});
  goUpLayer->getRenderer()->setRoundedBorderRadius(8);
  goUpLayer->onClick([this]() {
    uint layer_id = project->getMap()->getCurrentLayer()->getId();
    project->getMap()->layerDown(project->getMap()->getCurrentLayer()->getId());
    app_.getNetwork().layerDown(project->getId(),layer_id);
    refreshLayerList();
  });
  layerPanel_->add(goUpLayer);

  auto goDownLayer = tgui::Button::create("▲");
  goDownLayer->setSize("44%", "7%");
  goDownLayer->setPosition("51%", "68%");
  goDownLayer->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
  goDownLayer->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
  goDownLayer->getRenderer()->setTextColor(tgui::Color::White);
  goDownLayer->getRenderer()->setBorders({0});
  goDownLayer->getRenderer()->setRoundedBorderRadius(8);
  goDownLayer->onClick([this]() {
    uint layer_id = project->getMap()->getCurrentLayer()->getId();
    project->getMap()->layerUp(project->getMap()->getCurrentLayer()->getId());
    app_.getNetwork().layerUp(project->getId(),layer_id);
    refreshLayerList();
  });
  layerPanel_->add(goDownLayer);

  auto maskButton = tgui::Button::create("M/U");
  maskButton->setSize("90%", "7%");
  maskButton->setPosition("5%", "76%");
  maskButton->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
  maskButton->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
  maskButton->getRenderer()->setTextColor(tgui::Color::White);
  maskButton->getRenderer()->setBorders({0});
  maskButton->getRenderer()->setRoundedBorderRadius(8);
  maskButton->onClick([this]() {
    auto& layers = project->getMap()->getLayers();
    unsigned int selectedLayer = project->getMap()->getLayerSelected();
    if (selectedLayer < layers.size())
      layers[selectedLayer]->setMasked(!layers[selectedLayer]->isMasked());
  });
  layerPanel_->add(maskButton);
  maskButton->setTextSize(15);

  auto addLayerButton = tgui::Button::create("Ajouter couche");
  addLayerButton->setSize("90%", "7%");
  addLayerButton->setPosition("5%", "84%");
  addLayerButton->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
  addLayerButton->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
  addLayerButton->getRenderer()->setTextColor(tgui::Color::White);
  addLayerButton->getRenderer()->setBorders({0});
  addLayerButton->getRenderer()->setRoundedBorderRadius(8);
  addLayerButton->onClick([this]() {
    auto& gui = app_.getGui();

    auto popup = tgui::Panel::create();
    popup->setSize("22%", "22%");
    popup->setPosition("39%", "39%");
    popup->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
    popup->getRenderer()->setBorders({1});
    popup->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
    popup->getRenderer()->setRoundedBorderRadius(8);

    auto createPixelLayerButton = tgui::Button::create("Couche Pixel");
    createPixelLayerButton->setSize("73%", "32%");
    createPixelLayerButton->setPosition("14%", "14%");
    createPixelLayerButton->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
    createPixelLayerButton->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
    createPixelLayerButton->getRenderer()->setTextColor(tgui::Color::White);
    createPixelLayerButton->getRenderer()->setTextSize(13);
    createPixelLayerButton->getRenderer()->setBorders({0});
    createPixelLayerButton->getRenderer()->setRoundedBorderRadius(6);
    popup->add(createPixelLayerButton);

    auto createSpriteLayerButton = tgui::Button::create("Couche Sprite");
    createSpriteLayerButton->setSize("73%", "32%");
    createSpriteLayerButton->setPosition("14%", "50%");
    createSpriteLayerButton->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
    createSpriteLayerButton->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
    createSpriteLayerButton->getRenderer()->setTextColor(tgui::Color::White);
    createSpriteLayerButton->getRenderer()->setTextSize(13);
    createSpriteLayerButton->getRenderer()->setBorders({0});
    createSpriteLayerButton->getRenderer()->setRoundedBorderRadius(6);
    popup->add(createSpriteLayerButton);

    gui.add(popup);

    createPixelLayerButton->onClick([this, popup]() {
      auto& gui = app_.getGui();
      gui.remove(popup);
      createLayer(PIXELLAYER);
      app_.getNetwork().createLayer(project->getId(),PIXELLAYER);
    });

    createSpriteLayerButton->onClick([this, popup]() {
      auto& gui = app_.getGui();
      gui.remove(popup);
      createLayer(SPRITELAYER);
      app_.getNetwork().createLayer(project->getId(),SPRITELAYER);
    });
  });
  layerPanel_->add(addLayerButton);
  addLayerButton->setTextSize(15);

  auto removeLayerButton = tgui::Button::create("Supprimer couche");
  removeLayerButton->setSize("90%", "7%");
  removeLayerButton->setPosition("5%", "91%");
  removeLayerButton->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
  removeLayerButton->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
  removeLayerButton->getRenderer()->setTextColor(tgui::Color::White);
  removeLayerButton->getRenderer()->setBorders({0});
  removeLayerButton->getRenderer()->setRoundedBorderRadius(8);
  removeLayerButton->onClick([this]() {
    LayerType type = project->getMap()->getCurrentLayer()->getType();
    uint layer_id = project->getMap()->getCurrentLayer()->getId();
    project->getMap()->deleteLayer();
    checkTypeTool(type);
    refreshLayerList();
    app_.getNetwork().deleteLayer(project->getId(),layer_id);
  });
  layerPanel_->add(removeLayerButton);
  removeLayerButton->setTextSize(15);
}

void GameView::refreshLayerList() {
  layersList_->removeAllWidgets();
  const vector<shared_ptr<Layer>> &layers = project->getMap()->getLayers();
  unsigned int selected = project->getMap()->getLayerSelected();
  for (int i = layers.size()-1; i >= 0; i--) {
      auto layerButton = tgui::Button::create(layers[i]->getName());
      bool isSelected = (i == selected);
      layerButton->getRenderer()->setBackgroundColor(isSelected ? tgui::Color(60, 130, 200) : tgui::Color(36, 40, 47));
      layerButton->getRenderer()->setBackgroundColorHover(isSelected ? tgui::Color(75, 150, 220) : tgui::Color(50, 56, 66));
      layerButton->getRenderer()->setTextColor(tgui::Color::White);
      layerButton->getRenderer()->setBorders({1});
      layerButton->getRenderer()->setBorderColor(tgui::Color(70, 75, 85));
      layerButton->getRenderer()->setRoundedBorderRadius(6);
      layerButton->setSize("89%", "18%");
      layerButton->setPosition("5%", tgui::String(std::to_string(i * 19.76f) + "%"));
      layerButton->onClick([this, i]() {
        LayerType type = project->getMap()->getCurrentLayer()->getType();
        project->getMap()->selectLayer(static_cast<unsigned int>(i));
        checkTypeTool(type);
        refreshLayerList();
      });
      layersList_->add(layerButton);
      layerButton->setTextSize(15);
    }
  }


  void GameView::checkTypeTool(LayerType previous_type){
  LayerType current_type = project->getMap()->getCurrentLayer()->getType();

  if(previous_type == current_type)
    return;


  ToolBar& toolbar = project->getToolBar();
  ToolType tooltype = project->getToolBar().getSelected();

  switch (tooltype){
    case PIXELBRUSH:{
      bool erraser = static_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool())->getEraser();
      if(erraser){
        toolbar.selectTool(SPRITEERASER);
        if (eraserOptionsPanel_ && eraserOptionsPanel_->isVisible()) {
          eraserOptionsPanel_->setVisible(false);
          if (eraserSpriteOptionsPanel_) eraserSpriteOptionsPanel_->setVisible(true);
        }
      } else {
        toolbar.selectTool(SPRITEBRUSH);
        if (penOptionsPanel_ && penOptionsPanel_->isVisible()) {
          penOptionsPanel_->setVisible(false);
          if (penSpriteOptionsPanel_) penSpriteOptionsPanel_->setVisible(true);
        }
      }
      break;
    }
    case PIXELSHIFT:{
      toolbar.selectTool(SPRITESHIFT);
      break;
    }
    case SPRITEBRUSH:{
      toolbar.selectTool(PIXELBRUSH);
      static_pointer_cast<PixelBrush>(toolbar.getSelectedTool())->setEraser(false);
      if (penSpriteOptionsPanel_ && penSpriteOptionsPanel_->isVisible()) {
        penSpriteOptionsPanel_->setVisible(false);
        if (penOptionsPanel_) penOptionsPanel_->setVisible(true);
      } else if (spriteBrushOptionsPanel_) {
        spriteBrushOptionsPanel_->setVisible(false);
      }
      break;
    }
    case SPRITESHIFT:{
      toolbar.selectTool(PIXELSHIFT);
      break;
    }
    case NONETOOL:
      break;
    case SPRITEERASER :{
      toolbar.selectTool(PIXELBRUSH);
      static_pointer_cast<PixelBrush>(toolbar.getSelectedTool())->setEraser(true);
      if (eraserSpriteOptionsPanel_ && eraserSpriteOptionsPanel_->isVisible()) {
        eraserSpriteOptionsPanel_->setVisible(false);
        if (eraserOptionsPanel_) eraserOptionsPanel_->setVisible(true);
      }
      break;
    case SPRITESELECTION:{
        toolbar.selectTool(NONETOOL);
        break;
      }
    }
  }
}


  void GameView::createLayer(LayerType t){
      LayerType type = project->getMap()->getCurrentLayer()->getType();
      if(t == SPRITELAYER)
      project->getMap()->createSpriteLayer();
      else if(t==PIXELLAYER)
      project->getMap()->createPixelLayer();
      checkTypeTool(type);
      refreshLayerList();
}

  void GameView::deleteLayer(uint layer_id){
    LayerType type = project->getMap()->getCurrentLayer()->getType();
    project->getMap()->deleteLayer(layer_id);
    checkTypeTool(type);
    refreshLayerList();
}

void GameView::renameLayer(uint layer_id,std::string name){
project->getMap()->renameLayer(layer_id,name);
refreshLayerList();
}

void GameView::layerUp(uint layer_id){
  project->getMap()->layerUp(layer_id);
  refreshLayerList();
}
void GameView::layerDown(uint layer_id){
  project->getMap()->layerDown(layer_id);
  refreshLayerList();
}



void GameView::autoFill(uint layer_id,std::vector<std::string> asset_id, std::vector<sf::Vector2f> pos, float rotatation, float size){

  shared_ptr<Map> map = project->getMap();
  map->createSpriteLayer(layer_id);
  std::shared_ptr<Layer> layer = map->getLayers()[map->getLayerSelected() +1];
  if(layer->getType() == SPRITELAYER){
    std::shared_ptr<SpriteLayer> spritelayer = static_pointer_cast<SpriteLayer>(layer);
    AssetManager& assetManager = project->getMap()->getAssetManager();
    for(int i = 0; i< asset_id.size(); i++){
      std::string id = asset_id[i];
      Asset* asset = assetManager.getAsset(id);
      sf::Sprite sprite = sf::Sprite(*(asset->texture));
      
      sf::FloatRect bounds = sprite.getLocalBounds();
      sprite.setOrigin(sf::Vector2f(bounds.size.x / 2.0f, bounds.size.y / 2.0f));

      float scaleFactor = size*map->getScale() / bounds.size.x;
      sprite.setScale(sf::Vector2f(scaleFactor, scaleFactor));
      
      sprite.setPosition(pos[i]);
      sprite.setRotation(sf::degrees(rotatation));

      spritelayer->draw(sprite,id);
    }
    refreshLayerList();
  }
}
