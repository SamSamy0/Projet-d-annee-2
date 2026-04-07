// #include "../Window.hpp"
#include "../../project/Layer/layer.hpp"
#include "../../project/Tool/pixelbrush.hpp"
#include "../Application.hpp"
#include "GameView.hpp"
#include <memory>
#include <algorithm>

void GameView::initLayerPanel() {
  auto& mainWindow = app_.getWindow();
  auto& gui = app_.getGui();
  float width  = mainWindow.getSize().x;
  float height = mainWindow.getSize().y;

  // Création du panel qui reprend tous les éléments liés à l'affichage des couches et leur utilisation
  layerPanel_ = tgui::Panel::create();
  layerPanel_->setSize(width * 0.2, height * 0.58);
  layerPanel_->setPosition(width * 0.01, width * 0.04);
  layerPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  layerPanel_->getRenderer()->setBorders({1});
  layerPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  layerPanel_->getRenderer()->setRoundedBorderRadius(10);
  gui.add(layerPanel_);

  // Création du titre couches
  auto titleLabel = tgui::Label::create("Couches");
  titleLabel->setSize(width * 0.2, height * 0.05);
  titleLabel->setPosition(0, 0);
  titleLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
  titleLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
  titleLabel->getRenderer()->setTextColor(tgui::Color::White);
  layerPanel_->add(titleLabel);

  // Ici, je crée la box qui contient la liste des couches
  layersList_ = tgui::ScrollablePanel::create();
  layersList_->setSize(width * 0.18, height * 0.28);
  layersList_->setPosition(width * 0.01, height * 0.05);
  layersList_->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  layersList_->getRenderer()->setBorders({1});
  layersList_->getRenderer()->setBorderColor(tgui::Color(70, 75, 85));
  layersList_->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  layersList_->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  layerPanel_->add(layersList_);

  // J'itère sur chaque éléments du vecteur couches et je l'ajoute à la liste des couches en gérant son affichage
  vector<shared_ptr<Layer>> &layers = project->getMap()->getLayers();
  unsigned int selected = project->getMap()->getLayerSelected();
  for (size_t i = 0; i < layers.size(); i++) {
    auto layerButton = tgui::Button::create(layers[i]->getName());
    bool isSelected = (i == selected);
    layerButton->getRenderer()->setBackgroundColor(isSelected ? tgui::Color(60, 130, 200) : tgui::Color(36, 40, 47));
    layerButton->getRenderer()->setBackgroundColorHover(isSelected ? tgui::Color(75, 150, 220) : tgui::Color(50, 56, 66));
    layerButton->getRenderer()->setTextColor(tgui::Color::White);
    layerButton->getRenderer()->setBorders({1});
    layerButton->getRenderer()->setBorderColor(tgui::Color(70, 75, 85));
    layerButton->getRenderer()->setRoundedBorderRadius(6);
    layerButton->setSize(width * 0.16, height * 0.05);
    layerButton->setPosition(width * 0.01, i * height * 0.055);
    layerButton->onClick([this, i]() {
      LayerType type = project->getMap()->getCurrentLayer()->getType();
      project->getMap()->selectLayer(static_cast<unsigned int>(i));
      checkTypeTool(type);
      refreshLayerList();
    });
    layersList_->add(layerButton);
    layerButton->setTextSize(15);
  }

  // Création du bouton renommer une couche
  auto renameLayerButton = tgui::Button::create("Renommer");
  renameLayerButton->setSize(width * 0.18, height * 0.04);
  renameLayerButton->setPosition(width * 0.01, height * 0.345);
  renameLayerButton->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
  renameLayerButton->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
  renameLayerButton->getRenderer()->setTextColor(tgui::Color::White);
  renameLayerButton->getRenderer()->setBorders({0});
  renameLayerButton->getRenderer()->setRoundedBorderRadius(8);
  renameLayerButton->onClick([this]() {
    auto& mainWindow = app_.getWindow();
    auto& gui = app_.getGui();
    float width  = mainWindow.getSize().x;
    float height = mainWindow.getSize().y;

    auto popup = tgui::Panel::create();
    popup->setSize(width * 0.22, height * 0.18);
    popup->setPosition(width * 0.39, height * 0.41);
    popup->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
    popup->getRenderer()->setBorders({1});
    popup->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
    popup->getRenderer()->setRoundedBorderRadius(8);

    auto newLayerNameInput = tgui::EditBox::create();
    newLayerNameInput->setSize(width * 0.18, height * 0.06);
    newLayerNameInput->setPosition(width * 0.02, height * 0.02);
    auto& layers = project->getMap()->getLayers();
    unsigned int selectedLayer = project->getMap()->getLayerSelected();
    newLayerNameInput->setText(layers[selectedLayer]->getName());
    popup->add(newLayerNameInput);

    auto confirmNameButton = tgui::Button::create("OK");
    confirmNameButton->setSize(width * 0.08, height * 0.06);
    confirmNameButton->setPosition(width * 0.02, height * 0.1);
    confirmNameButton->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
    confirmNameButton->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
    confirmNameButton->getRenderer()->setTextColor(tgui::Color::White);
    confirmNameButton->getRenderer()->setBorders({0});
    confirmNameButton->getRenderer()->setRoundedBorderRadius(6);
    popup->add(confirmNameButton);

    auto cancelNameButton = tgui::Button::create("Annuler");
    cancelNameButton->setSize(width * 0.08, height * 0.06);
    cancelNameButton->setPosition(width * 0.12, height * 0.1);
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

  // Création du bouton qui décale une couche vers le haut
  auto goUpLayer = tgui::Button::create("▼");
  goUpLayer->setSize(width * 0.0875, height * 0.04);
  goUpLayer->setPosition(width * 0.01, height * 0.395);
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

  // Création du bouton qui décale une couche vers le bas
  auto goDownLayer = tgui::Button::create("▲");
  goDownLayer->setSize(width * 0.0875, height * 0.04);
  goDownLayer->setPosition(width * 0.1025, height * 0.395);
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
  
  // Création du toggle bouton masqué/démasqué
  auto maskButton = tgui::Button::create("M/U");
  maskButton->setSize(width * 0.18, height * 0.04);
  maskButton->setPosition(width * 0.01, height * 0.44);
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

  // Création du bouton ajouter une couche (pour l'instant direct couche pixel)
  auto addLayerButton = tgui::Button::create("Ajouter couche");
  addLayerButton->setSize(width * 0.18, height * 0.04);
  addLayerButton->setPosition(width * 0.01, height * 0.485);
  addLayerButton->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
  addLayerButton->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
  addLayerButton->getRenderer()->setTextColor(tgui::Color::White);
  addLayerButton->getRenderer()->setBorders({0});
  addLayerButton->getRenderer()->setRoundedBorderRadius(8);
  addLayerButton->onClick([this]() {
    auto& mainWindow = app_.getWindow();
    auto& gui = app_.getGui();
    float width  = mainWindow.getSize().x;
    float height = mainWindow.getSize().y;

    auto popup = tgui::Panel::create();
    popup->setSize(width * 0.22f, height * 0.22f);
    popup->setPosition(width * 0.39f, height * 0.39f);
    popup->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
    popup->getRenderer()->setBorders({1});
    popup->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
    popup->getRenderer()->setRoundedBorderRadius(8);

    auto createPixelLayerButton = tgui::Button::create("Couche Pixel");
    createPixelLayerButton->setSize(width * 0.16f, height * 0.07f);
    createPixelLayerButton->setPosition(width * 0.03f, height * 0.03f);
    createPixelLayerButton->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
    createPixelLayerButton->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
    createPixelLayerButton->getRenderer()->setTextColor(tgui::Color::White);
    createPixelLayerButton->getRenderer()->setTextSize(13);
    createPixelLayerButton->getRenderer()->setBorders({0});
    createPixelLayerButton->getRenderer()->setRoundedBorderRadius(6);
    popup->add(createPixelLayerButton);

    auto createSpriteLayerButton = tgui::Button::create("Couche Sprite");
    createSpriteLayerButton->setSize(width * 0.16f, height * 0.07f);
    createSpriteLayerButton->setPosition(width * 0.03f, height * 0.11f);
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

  // Création du bouton supprimer une couche
  auto removeLayerButton = tgui::Button::create("Supprimer couche");
  removeLayerButton->setSize(width * 0.18, height * 0.04);
  removeLayerButton->setPosition(width * 0.01, height * 0.53);
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
  auto& mainWindow = app_.getWindow();
  auto& gui = app_.getGui();
  float width  = mainWindow.getSize().x;
  float height = mainWindow.getSize().y;

  layersList_->removeAllWidgets();
  const vector<shared_ptr<Layer>> &layers = project->getMap()->getLayers();
  unsigned int selected = project->getMap()->getLayerSelected();
  for (size_t i = 0; i < layers.size(); i++) {
    auto layerButton = tgui::Button::create(layers[i]->getName());
    bool isSelected = (i == selected);
    layerButton->getRenderer()->setBackgroundColor(isSelected ? tgui::Color(60, 130, 200) : tgui::Color(36, 40, 47));
    layerButton->getRenderer()->setBackgroundColorHover(isSelected ? tgui::Color(75, 150, 220) : tgui::Color(50, 56, 66));
    layerButton->getRenderer()->setTextColor(tgui::Color::White);
    layerButton->getRenderer()->setBorders({1});
    layerButton->getRenderer()->setBorderColor(tgui::Color(70, 75, 85));
    layerButton->getRenderer()->setRoundedBorderRadius(6);
    layerButton->setSize(width * 0.16, height * 0.05);
    layerButton->setPosition(width * 0.01, i * height * 0.055);
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
