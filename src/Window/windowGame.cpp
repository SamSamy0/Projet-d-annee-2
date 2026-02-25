#include "displayWindow.hpp"

void Window::initWidget() {
  gui.removeAllWidgets();
  updateTextSize();
  gui.onViewChange([this] { updateTextSize(); });

  if (this->state == projectState::LOGIN) {
    Window::loginWidget();
  } else if (this->state == projectState::MENU) {
    Window::initMenuWidget();
  } else if (this->state == projectState::GAME) {
    // Ajout du bouton de Home (retour en arrière)
    auto homeButton = tgui::Button::create();
    homeButton->setSize(30, 30);
    homeButton->setPosition(15, 5);
    homeButton->getRenderer()->setTexture("../res/images/accueil.png");
    homeButton->getRenderer()->setBorders({0});
    homeButton->onPress(&Window::setState, this, projectState::MENU);
    gui.add(homeButton);

    // Ajout du bouton de zoom in
    auto zoomInButton = tgui::Button::create();
    zoomInButton->setSize(30, 30);
    zoomInButton->setPosition(80, 5);
    zoomInButton->getRenderer()->setTexture("../res/images/zoom-avant.png");
    zoomInButton->getRenderer()->setBorders({0});
    zoomInButton->onPress([this]() { project->getMap()->getZoom().zoomIn(); });
    gui.add(zoomInButton);

    // Ajout du bouton de zoom out
    auto zoomOutButton = tgui::Button::create();
    zoomOutButton->setSize(30, 30);
    zoomOutButton->setPosition(125, 5);
    zoomOutButton->getRenderer()->setTexture("../res/images/zoom-arriere.png");
    zoomOutButton->getRenderer()->setBorders({0});
    zoomOutButton->onPress(
        [this]() { project->getMap()->getZoom().zoomOut(); });
    gui.add(zoomOutButton);

    // Ajout boutton crayon
    auto penButton = tgui::Button::create();
    penButton->setSize(25, 25);
    penButton->setPosition(350, 9);
    penButton->getRenderer()->setTexture("../res/images/crayon.png");
    penButton->getRenderer()->setBorders({0});
    penButton->onPress([this]() {
      project->getToolBar().selectTool(PIXELBRUSH);
      project->getToolBar().getSelectedTool()->setErraser(false);
    });
    gui.add(penButton);

    // Ajout boutton gomme
    auto brushButton = tgui::Button::create();
    brushButton->setSize(30, 30);
    brushButton->setPosition(420, 13);
    brushButton->getRenderer()->setTexture("../res/images/la-gomme.png");
    brushButton->getRenderer()->setBorders({0});
    brushButton->onPress([this]() {
      project->getToolBar().selectTool(PIXELBRUSH);
      project->getToolBar().getSelectedTool()->setErraser(true);
    });
    gui.add(brushButton);

    // Ajout boutton deplacement couche
    auto shiftButton = tgui::Button::create();
    shiftButton->setSize(30, 30);
    shiftButton->setPosition(490, 13);
    shiftButton->getRenderer()->setTexture("../res/images/couche.png");
    shiftButton->getRenderer()->setBorders({0});
    shiftButton->onPress(
        [this]() { project->getToolBar().selectTool(PIXELSHIFT); });
    gui.add(shiftButton);

    // Ajout de la liste des couches
    layersList_ = tgui::ScrollablePanel::create();
    layersList_->setSize(mainWindow.getSize().x * 0.18,
                         mainWindow.getSize().y * 0.28);
    layersList_->setPosition(mainWindow.getSize().x * 0.02,
                             mainWindow.getSize().y * 0.14);
    layersList_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
    // layersList_->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);

    vector<shared_ptr<Layer>> &layers = project->getMap()->getLayers();
    for (size_t i = 0; i < layers.size(); i++) {
      auto layerButton =
          tgui::Button::create("Couche " + std::to_string(i + 1));
      layerButton->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
      layerButton->getRenderer()->setTextColor(tgui::Color::White);
      layerButton->getRenderer()->setBorders({2});
      layerButton->getRenderer()->setBorderColor(tgui::Color(36, 40, 47));
      layerButton->setSize(240, 40);
      layerButton->setPosition(10, i * 50);
      layerButton->onClick([this, i]() {
        project->getMap()->setLayerSelected(static_cast<unsigned int>(i));
      });
      layersList_->add(layerButton);
    }

    layersList_->setContentSize(sf::Vector2f(300, layers.size() * 50));
    gui.add(layersList_);

    // Ajout du bouton Créer une couche
    auto addLayerButton = tgui::Button::create("Ajouter couche");
    addLayerButton->setSize({mainWindow.getSize().x * 0.165, 40});
    addLayerButton->setPosition(mainWindow.getSize().x * 0.03,
                                mainWindow.getSize().y * 0.50);
    addLayerButton->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
    addLayerButton->getRenderer()->setBackgroundColorHover(
        tgui::Color(65, 72, 84));
    addLayerButton->getRenderer()->setTextColor(tgui::Color::White);
    addLayerButton->getRenderer()->setBorders({1});
    addLayerButton->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
    addLayerButton->onClick([this]() {
      project->getMap()->createPixelLayer();
      refreshLayerList();
    });
    gui.add(addLayerButton);
    addLayerButton->setTextSize(19);

    // Ajout du bouton supprimer une couche
    auto removeLayerButton = tgui::Button::create("Supprimer couche");
    removeLayerButton->setSize({mainWindow.getSize().x * 0.165, 40});
    removeLayerButton->setPosition(mainWindow.getSize().x * 0.03,
                                   mainWindow.getSize().y * 0.55);
    removeLayerButton->getRenderer()->setBackgroundColor(
        tgui::Color(50, 56, 66));
    removeLayerButton->getRenderer()->setBackgroundColorHover(
        tgui::Color(65, 72, 84));
    removeLayerButton->getRenderer()->setTextColor(tgui::Color::White);
    removeLayerButton->getRenderer()->setBorders({1});
    removeLayerButton->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
    removeLayerButton->onClick([this]() {
      vector<shared_ptr<Layer>> &layers = project->getMap()->getLayers();
      if (layers.size() <= 1)
        return;
      layers.erase(layers.begin() + project->getMap()->getLayerSelected());
      unsigned int newLayerSelected = project->getMap()->getLayerSelected() - 1;
      project->getMap()->setLayerSelected(newLayerSelected);
      refreshLayerList();
    });
    gui.add(removeLayerButton);
    removeLayerButton->setTextSize(19);
  }
}
