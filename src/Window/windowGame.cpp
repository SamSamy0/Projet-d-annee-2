#include "Window.hpp"

void Window::initGameWidget() {
  initToolbar();
  initLayerPanel();
}

void Window::initToolbar() {
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
  zoomOutButton->onPress([this]() { project->getMap()->getZoom().zoomOut(); });
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
}

void Window::initLayerPanel() {
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
    auto layerButton = tgui::Button::create("Couche " + std::to_string(i + 1));
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
  removeLayerButton->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
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

void Window::refreshLayerList() {
  layersList_->removeAllWidgets();
  const vector<shared_ptr<Layer>> &layers = project->getMap()->getLayers();
  for (size_t i = 0; i < layers.size(); i++) {
    auto layerButton = tgui::Button::create("Couche " + std::to_string(i + 1));
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
}

void Window::handleGameEvents(const std::optional<sf::Event> &event) {
  project->getMap()->detectMovement();
  if (const auto *mousePressed =
          event->getIf<sf::Event::MouseButtonPressed>()) {
    sf::Vector2i mousePos = mousePressed->position; // mouse position
    if (!gui.getWidgetBelowMouseCursor(mousePos, true)) {
      sf::Vector2f pos =
          mainWindow.mapPixelToCoords(mousePos, project->getView());
      sf::Vector2i mapPos(static_cast<int>(pos.x), static_cast<int>(pos.y));
      project->getToolBar().getSelectedTool()->onPress(mapPos);
    }
  }

  if (auto mouseEvent = event->getIf<sf::Event::MouseButtonReleased>()) {
    if (mouseEvent->button == sf::Mouse::Button::Left) {
      if (!gui.getWidgetBelowMouseCursor(mouseEvent->position, true)) {
        project->getToolBar().getSelectedTool()->onRelease();
      }
    }
  }

  if (const auto *mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
    sf::Vector2i mousePos = mouseMoved->position; // mouse position
    if (!gui.getWidgetBelowMouseCursor(mousePos, true)) {
      sf::Vector2f pos =
          mainWindow.mapPixelToCoords(mousePos, project->getView());
      sf::Vector2i mapPos(static_cast<int>(pos.x), static_cast<int>(pos.y));
      project->getToolBar().getSelectedTool()->onDrag(mapPos);
    }
  }

  // SET SIZE
  if (const auto *wheelEvent = event->getIf<sf::Event::MouseWheelScrolled>()) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
      // toolType tool = project->getToolBar().getSelected();
      // if (tool == PIXELBRUSH || tool == SPRITEBRUSH) {
      //   sf::Vector2u size =
      //       project->getToolBar().getSelectedTool()->getSize();
      //   float sizex = size.x;
      //   float sizey = size.y;
      //   sf::Vector2u mapSize = project->getMap()->getSize();
      //   float step = std::min(mapSize.x, mapSize.y) / 100.0f;
      //
      //   if (wheelEvent->delta > 0) {
      //     sizex += 1;
      //     sizey += 1;
      //   } else if (wheelEvent->delta < 0) {
      //     sizex -= 1;
      //     sizey -= 1;
      //   }
      //
      //   if (sizex < 1) {
      //     sizex = 1;
      //   } else if (sizex > mapSize.x) {
      //     sizex = mapSize.x;
      //   }
      //   if (sizey < 1) {
      //     sizey = 1;
      //   } else if (sizey > mapSize.y) {
      //     sizey = mapSize.y;
      //   }
      //
      //   size = sf::Vector2u(static_cast<unsigned int>(std::round(sizex)),
      //                       static_cast<unsigned
      //                       int>(std::round(sizey)));
      //   project->getToolBar().getSelectedTool()->setSize(size.x, size.y);
      // }
    } else {
      if (!gui.getWidgetBelowMouseCursor(wheelEvent->position, true))
        project->getMap()->zooming(wheelEvent); // ZOOM
    }
  }
}
