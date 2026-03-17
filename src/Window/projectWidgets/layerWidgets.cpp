#include "../Window.hpp"
#include "../../project/Layer/layer.hpp"
#include "../../project/Tool/pixelbrush.hpp"
#include <memory>
#include <algorithm>

void Window::initLayerPanel() {
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
  layersList_->setSize(width * 0.18, height * 0.33);
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
      project->getMap()->setLayerSelected(static_cast<unsigned int>(i));
      checkTypeTool(type);
      refreshLayerList();
    });
    layersList_->add(layerButton);
  }

  // Création du bouton qui décale une couche vers le haut
  auto goUpLayer = tgui::Button::create("▼");
  goUpLayer->setSize(width * 0.08, height * 0.04);
  goUpLayer->setPosition(width * 0.01, height * 0.395);
  goUpLayer->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
  goUpLayer->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
  goUpLayer->getRenderer()->setTextColor(tgui::Color::White);
  goUpLayer->getRenderer()->setBorders({0});
  goUpLayer->getRenderer()->setRoundedBorderRadius(8);
  goUpLayer->onClick([this]() {
    auto& layers = project->getMap()->getLayers();
    unsigned int selectedLayer = project->getMap()->getLayerSelected();
    if (selectedLayer == layers.size() - 1) return;
    std::swap(layers[selectedLayer], layers[selectedLayer + 1]);
    project->getMap()->setLayerSelected(selectedLayer + 1);
    refreshLayerList();
  });
  layerPanel_->add(goUpLayer);

  // Création du bouton qui décale une couche vers le bas
  auto goDownLayer = tgui::Button::create("▲");
  goDownLayer->setSize(width * 0.08, height * 0.04);
  goDownLayer->setPosition(width * 0.11, height * 0.395);
  goDownLayer->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
  goDownLayer->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
  goDownLayer->getRenderer()->setTextColor(tgui::Color::White);
  goDownLayer->getRenderer()->setBorders({0});
  goDownLayer->getRenderer()->setRoundedBorderRadius(8);
  goDownLayer->onClick([this]() {
    auto& layers = project->getMap()->getLayers();
    unsigned int selectedLayer = project->getMap()->getLayerSelected();
    if (selectedLayer == 0) return;
    std::swap(layers[selectedLayer], layers[selectedLayer - 1]);
    project->getMap()->setLayerSelected(selectedLayer - 1);
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
      layers[selectedLayer]->setMasked(!layers[selectedLayer]->getMasked());
  });
  layerPanel_->add(maskButton);

  // Création du toggle bouton lock/unlock
  //auto lockButton = tgui::Button::create("L/U");
  //lockButton->setSize(width * 0.09, height * 0.04);
  //lockButton->setPosition(width * 0.1, height * 0.44);
  //lockButton->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
  //lockButton->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
  //lockButton->getRenderer()->setTextColor(tgui::Color::White);
  //lockButton->getRenderer()->setBorders({0});
  //lockButton->getRenderer()->setRoundedBorderRadius(8);
  //lockButton->onClick([this]() {
  // auto& layers = project->getMap()->getLayers();
  //  unsigned int selectedLayer = project->getMap()->getLayerSelected();
  //  if (selectedLayer < layers.size())
  //    layers[selectedLayer]->setMasked(!layers[selectedLayer]->getMasked());
  //});
  //layerPanel_->add(lockButton);
  

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
    float width  = mainWindow.getSize().x;
    float height = mainWindow.getSize().y;

    auto popup = tgui::Panel::create();
    popup->setSize(width * 0.22f, height * 0.22f);
    popup->setPosition(width * 0.39f, height * 0.39f);
    popup->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
    popup->getRenderer()->setBorders({1});
    popup->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
    popup->getRenderer()->setRoundedBorderRadius(8);

    auto pixelBtn = tgui::Button::create("Couche Pixel");
    pixelBtn->setSize(width * 0.16f, height * 0.07f);
    pixelBtn->setPosition(width * 0.03f, height * 0.03f);
    pixelBtn->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
    pixelBtn->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
    pixelBtn->getRenderer()->setTextColor(tgui::Color::White);
    pixelBtn->getRenderer()->setTextSize(13);
    pixelBtn->getRenderer()->setBorders({0});
    pixelBtn->getRenderer()->setRoundedBorderRadius(6);
    popup->add(pixelBtn);

    auto spriteBtn = tgui::Button::create("Couche Sprite");
    spriteBtn->setSize(width * 0.16f, height * 0.07f);
    spriteBtn->setPosition(width * 0.03f, height * 0.11f);
    spriteBtn->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
    spriteBtn->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
    spriteBtn->getRenderer()->setTextColor(tgui::Color::White);
    spriteBtn->getRenderer()->setTextSize(13);
    spriteBtn->getRenderer()->setBorders({0});
    spriteBtn->getRenderer()->setRoundedBorderRadius(6);
    popup->add(spriteBtn);

    gui.add(popup);

    pixelBtn->onClick([this, popup]() {
      LayerType type = project->getMap()->getCurrentLayer()->getType();
      project->getMap()->createPixelLayer();
      checkTypeTool(type);
      refreshLayerList();
      gui.remove(popup);
    });

    spriteBtn->onClick([this, popup]() {
      LayerType type = project->getMap()->getCurrentLayer()->getType();
      project->getMap()->createSpriteLayer();
      checkTypeTool(type);
      refreshLayerList();
      gui.remove(popup);
    });
  });
  layerPanel_->add(addLayerButton);

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
    vector<shared_ptr<Layer>> &layers = project->getMap()->getLayers();
    if (layers.size() <= 1) return;
    LayerType type = project->getMap()->getCurrentLayer()->getType();
    unsigned int selected = project->getMap()->getLayerSelected();
    layers.erase(layers.begin() + selected);
    unsigned int newSelected = (selected > 0) ? selected - 1 : 0;
    project->getMap()->setLayerSelected(newSelected);
    checkTypeTool(type);
    refreshLayerList();
  });
  layerPanel_->add(removeLayerButton);
}

void Window::refreshLayerList() {
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
      project->getMap()->setLayerSelected(static_cast<unsigned int>(i));
      checkTypeTool(type);
      refreshLayerList();
    });
    layersList_->add(layerButton);
  }
}


  void Window::checkTypeTool(LayerType previous_type){
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
      }
      else{toolbar.selectTool(SPRITEBRUSH);}

      break;
    }
    case PIXELSHIFT:{
      toolbar.selectTool(SPRITESHIFT);
      break;
    }
    case SPRITEBRUSH:{
      toolbar.selectTool(PIXELBRUSH);
      static_pointer_cast<PixelBrush>(toolbar.getSelectedTool())->setEraser(false);
      break;
    }
    case SPRITESHIFT:{
      toolbar.selectTool(PIXELSHIFT);
      break;
    }
    case SPRITEERASER :{
      toolbar.selectTool(PIXELBRUSH);
      static_pointer_cast<PixelBrush>(toolbar.getSelectedTool())->setEraser(true);
      break;
    }
  }
}
