#include "../Window.hpp"
#include "../../project/Layer/layer.hpp"

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
  titleLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
  titleLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
  titleLabel->getRenderer()->setTextColor(tgui::Color::White);
  layerPanel_->add(titleLabel);

  // Ici, je crée la box qui contient la liste des couches
  layersList_ = tgui::ScrollablePanel::create();
  layersList_->setSize(width * 0.18, height * 0.38);
  layersList_->setPosition(width * 0.01, height * 0.05);
  layersList_->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  layersList_->getRenderer()->setBorders({1});
  layersList_->getRenderer()->setBorderColor(tgui::Color(70, 75, 85));
  layersList_->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
  layersList_->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
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
      project->getMap()->setLayerSelected(static_cast<unsigned int>(i));
      refreshLayerList();
    });
    layersList_->add(layerButton);
  }
  // Bouton "M/U" (Masquer / Démasquer)
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
    unsigned int sel = project->getMap()->getLayerSelected();
    if (sel < layers.size())
      layers[sel]->setMasked(!layers[sel]->getMasked());
  });
  layerPanel_->add(maskButton);

  // Bouton "Ajouter couche"
  auto addLayerButton = tgui::Button::create("Ajouter couche");
  addLayerButton->setSize(width * 0.18, height * 0.04);
  addLayerButton->setPosition(width * 0.01, height * 0.485);
  addLayerButton->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
  addLayerButton->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
  addLayerButton->getRenderer()->setTextColor(tgui::Color::White);
  addLayerButton->getRenderer()->setBorders({0});
  addLayerButton->getRenderer()->setRoundedBorderRadius(8);
  addLayerButton->onClick([this]() {
    project->getMap()->createPixelLayer();
    refreshLayerList();
  });
  layerPanel_->add(addLayerButton);

  // Bouton "Supprimer couche"
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
    layers.erase(layers.begin() + project->getMap()->getLayerSelected());
    unsigned int newLayerSelected = project->getMap()->getLayerSelected() - 1;
    project->getMap()->setLayerSelected(newLayerSelected);
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
      project->getMap()->setLayerSelected(static_cast<unsigned int>(i));
      refreshLayerList();
    });
    layersList_->add(layerButton);
  }
}