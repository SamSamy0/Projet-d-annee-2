#include "Window.hpp"
#include "../project/Tool/pixelbrush.hpp"
#include "../project/Tool/pixelshift.hpp"
#include "../project/Chat/userMessage.hpp"

void Window::initGameWidget() {
  initToolbar();
  initLayerPanel();
  initChatWidget();
  initPenOptions();
  initEraserOptions();
  refreshChat();
}

void Window::initToolbar() {
  float width  = mainWindow.getSize().x;
  float height = mainWindow.getSize().y;

  auto toolbar = tgui::Panel::create();
  toolbar->setSize(width, height * 0.05);
  toolbar->setPosition(width * 0.0, height * 0.0);
  toolbar->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  toolbar->getRenderer()->setBorders({0});
  gui.add(toolbar, "toolbar");

  // Ajout du bouton de Home (retour en arrière)
  auto homeButton = tgui::Button::create();
  homeButton->setSize(height * 0.035, height * 0.035);
  homeButton->setPosition(width * 0.008, height * 0.007);
  homeButton->getRenderer()->setTexture("../res/images/accueil.png");
  homeButton->getRenderer()->setBorders({0});
  homeButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  homeButton->onPress(&Window::setState, this, projectState::MENU);
  toolbar->add(homeButton);

  // Ajout du bouton de zoom in
  auto zoomInButton = tgui::Button::create();
  zoomInButton->setSize(height * 0.035, height * 0.035);
  zoomInButton->setPosition(width * 0.07, height * 0.007);
  zoomInButton->getRenderer()->setTexture("../res/images/zoom-avant.png");
  zoomInButton->getRenderer()->setBorders({0});
  zoomInButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  zoomInButton->onPress([this]() { project->getMap()->getZoom().zoomIn(); });
  toolbar->add(zoomInButton);

  // Ajout du bouton de zoom out
  auto zoomOutButton = tgui::Button::create();
  zoomOutButton->setSize(height * 0.035, height * 0.035);
  zoomOutButton->setPosition(width * 0.12, height * 0.007);
  zoomOutButton->getRenderer()->setTexture("../res/images/zoom-arriere.png");
  zoomOutButton->getRenderer()->setBorders({0});
  zoomOutButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  zoomOutButton->onPress([this]() { project->getMap()->getZoom().zoomOut(); });
  toolbar->add(zoomOutButton);

  auto penButton   = tgui::Button::create();
  auto brushButton = tgui::Button::create();
  auto shiftButton = tgui::Button::create();

  penButton->setSize(height * 0.035, height * 0.035);
  penButton->setPosition(width * 0.25, height * 0.007);
  penButton->getRenderer()->setTexture("../res/images/crayon.png");
  penButton->getRenderer()->setBorders({0});
  penButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  penButton->getRenderer()->setOpacity(0.4);
  penButton->onPress([this, penButton, brushButton, shiftButton]() {
    project->getToolBar().selectTool(PIXELBRUSH);
    dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool())->setErraser(false);
    penButton->getRenderer()->setOpacity(1.0);
    brushButton->getRenderer()->setOpacity(0.4);
    shiftButton->getRenderer()->setOpacity(0.4);
    if (penOptionsPanel_) penOptionsPanel_->setVisible(!penOptionsPanel_->isVisible());
    if (eraserOptionsPanel_) eraserOptionsPanel_->setVisible(false);
  });
  toolbar->add(penButton);

  brushButton->setSize(height * 0.035, height * 0.035);
  brushButton->setPosition(width * 0.35, height * 0.007);
  brushButton->getRenderer()->setTexture("../res/images/la-gomme.png");
  brushButton->getRenderer()->setBorders({0});
  brushButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  brushButton->getRenderer()->setOpacity(0.4);
  brushButton->onPress([this, penButton, brushButton, shiftButton]() {
    project->getToolBar().selectTool(PIXELBRUSH);
    dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool())->setErraser(true);
    penButton->getRenderer()->setOpacity(0.4);
    brushButton->getRenderer()->setOpacity(1.0);
    shiftButton->getRenderer()->setOpacity(0.4);
    if (penOptionsPanel_) penOptionsPanel_->setVisible(false);
    if (eraserOptionsPanel_) eraserOptionsPanel_->setVisible(!eraserOptionsPanel_->isVisible());
  });
  toolbar->add(brushButton);

  shiftButton->setSize(height * 0.035, height * 0.035);
  shiftButton->setPosition(width * 0.45, height * 0.007);
  shiftButton->getRenderer()->setTexture("../res/images/couche.png");
  shiftButton->getRenderer()->setBorders({0});
  shiftButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  shiftButton->getRenderer()->setOpacity(0.4);
  shiftButton->onPress([this, penButton, brushButton, shiftButton]() {
    project->getToolBar().selectTool(PIXELSHIFT);
    penButton->getRenderer()->setOpacity(0.4);
    brushButton->getRenderer()->setOpacity(0.4);
    shiftButton->getRenderer()->setOpacity(1.0);
    if (penOptionsPanel_) penOptionsPanel_->setVisible(false);
    if (eraserOptionsPanel_) eraserOptionsPanel_->setVisible(false);
  });
  toolbar->add(shiftButton);
}

void Window::initLayerPanel() {
  float width  = mainWindow.getSize().x;
  float height = mainWindow.getSize().y;

  // Panneau principal
  layerPanel_ = tgui::Panel::create();
  layerPanel_->setSize(width * 0.2, height * 0.58);
  layerPanel_->setPosition(width * 0.01, width * 0.04);
  layerPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  layerPanel_->getRenderer()->setBorders({1});
  layerPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  layerPanel_->getRenderer()->setRoundedBorderRadius(10);
  gui.add(layerPanel_);

  // Titre "Couches"
  auto titleLabel = tgui::Label::create("Couches");
  titleLabel->setSize(width * 0.2, height * 0.05);
  titleLabel->setPosition(0, 0);
  titleLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
  titleLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
  titleLabel->getRenderer()->setTextColor(tgui::Color::White);
  layerPanel_->add(titleLabel);

  // Liste des couches
  layersList_ = tgui::ScrollablePanel::create();
  layersList_->setSize(width * 0.18, height * 0.42);
  layersList_->setPosition(width * 0.01, height * 0.05);
  layersList_->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  layersList_->getRenderer()->setBorders({1});
  layersList_->getRenderer()->setBorderColor(tgui::Color(70, 75, 85));
  layersList_->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
  layersList_->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
  layerPanel_->add(layersList_);

  vector<shared_ptr<Layer>> &layers = project->getMap()->getLayers();
  unsigned int selected = project->getMap()->getLayerSelected();
  for (size_t i = 0; i < layers.size(); i++) {
    auto layerButton = tgui::Button::create("Couche " + std::to_string(i + 1));
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
  layersList_->setContentSize(sf::Vector2f(width * 0.17, layers.size() * height * 0.055));

  // Bouton "Ajouter couche"
  auto addLayerButton = tgui::Button::create("Ajouter couche");
  addLayerButton->setSize(width * 0.18, height * 0.04);
  addLayerButton->setPosition(width * 0.01, height * 0.48);
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
    auto layerButton = tgui::Button::create("Couche " + std::to_string(i + 1));
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
  layersList_->setContentSize(sf::Vector2f(width * 0.17, layers.size() * height * 0.055));
}

void Window::handleGameEvents(const std::optional<sf::Event> &event) {
  if (!chatInput_->isFocused())
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
      //   float step = std::min(mapSize.x, mapSize.y) / 100.0;
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

void Window::initChatWidget() {
  float width  = mainWindow.getSize().x;
  float height = mainWindow.getSize().y;

  // Je crée le groupe de widgets pour le Chat
  chatPanel_ = tgui::Panel::create();
  chatPanel_->setSize(width * 0.2, height * 0.35);
  chatPanel_->setPosition(width * 0.01, height * 0.65);
  chatPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  chatPanel_->getRenderer()->setBorders({1});
  chatPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  chatPanel_->getRenderer()->setRoundedBorderRadius(10);
  gui.add(chatPanel_);

  // Titre "Chat"
  auto chatTitle = tgui::Label::create("Chat");
  chatTitle->setSize(width * 0.2, height * 0.045);
  chatTitle->setPosition(0, 0);
  chatTitle->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
  chatTitle->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
  chatTitle->getRenderer()->setTextColor(tgui::Color::White);
  chatPanel_->add(chatTitle);

  // Zone des messages (ScrollablePanel)
  chatMessages_ = tgui::ScrollablePanel::create();
  chatMessages_->setSize(width * 0.18, height * 0.194);
  chatMessages_->setPosition(width * 0.01, height * 0.045);
  chatMessages_->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  chatMessages_->getRenderer()->setBorders({1});
  chatMessages_->getRenderer()->setBorderColor(tgui::Color(70, 75, 85));
  chatMessages_->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
  chatMessages_->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
  chatPanel_->add(chatMessages_);

  // EditBox "Entrez message"
  chatInput_ = tgui::EditBox::create();
  chatInput_->setSize(width * 0.18, height * 0.05);
  chatInput_->setPosition(width * 0.01, height * 0.247);
  chatInput_->setDefaultText("Entrez message");
  chatInput_->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  chatInput_->getRenderer()->setBackgroundColorHover(tgui::Color(36, 40, 47));
  chatInput_->getRenderer()->setTextColor(tgui::Color::White);
  chatInput_->getRenderer()->setDefaultTextColor(tgui::Color(130, 135, 145));
  chatInput_->getRenderer()->setBorders({1});
  chatInput_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  chatPanel_->add(chatInput_);

  // Bouton "Envoyer"
  chatSendButton_ = tgui::Button::create("Envoyer");
  chatSendButton_->setSize(width * 0.18, height * 0.04);
  chatSendButton_->setPosition(width * 0.01, height * 0.302);
  chatSendButton_->setTextSize(12);
  chatSendButton_->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
  chatSendButton_->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
  chatSendButton_->getRenderer()->setTextColor(tgui::Color::White);
  chatSendButton_->getRenderer()->setBorders({0});
  chatSendButton_->getRenderer()->setRoundedBorderRadius(8);
  chatSendButton_->onPress([this]() {
    tgui::String text = chatInput_->getText();
    if (text.empty()) return;
    auto msg = std::make_shared<UserMessage>(currentUser_.getUser(), text.toStdString());
    project->getChat().addMessage(msg);
    chatInput_->setText("");
    refreshChat();
  });
  chatPanel_->add(chatSendButton_);
}

void Window::refreshChat() {
  float width  = mainWindow.getSize().x;
  float height = mainWindow.getSize().y;

  chatMessages_->removeAllWidgets();

  const auto& messages = project->getChat().getMessages();
  float pos_y = 4.0;
  const float authorLabelHeight = height * 0.022;

  for (const auto& msg : messages) {
    if (msg->getType() == MessageType::USER) {
      Date date = msg->getDate();
      std::string pseudo = msg->getAuthor().getUser();
      if (pseudo.empty()) pseudo = "Utilisateur";
      std::string header = pseudo + " - " + std::to_string(date.day_) + "/" + std::to_string(date.month_) + "/" + std::to_string(date.year_);

      auto authorLabel = tgui::Label::create();
      authorLabel->setText(header);
      authorLabel->setPosition(width * 0.005, pos_y);
      authorLabel->getRenderer()->setTextColor(tgui::Color(150, 160, 180));
      authorLabel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
      chatMessages_->add(authorLabel);
      authorLabel->setTextSize(15);
      pos_y += authorLabelHeight;

      auto messageLabel = tgui::Label::create(msg->getTexte());
      messageLabel->setPosition(width * 0.005, pos_y);
      messageLabel->getRenderer()->setTextColor(tgui::Color::White);
      messageLabel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
      messageLabel->setMaximumTextWidth(width * 0.16);
      chatMessages_->add(messageLabel);
      pos_y += messageLabel->getSize().y + 4.0;
    }
  }

  chatMessages_->setContentSize({static_cast<float>(width * 0.18), pos_y});
  chatMessages_->setVerticalScrollbarValue(chatMessages_->getVerticalScrollbarMaxValue());
}

void Window::initPenOptions() {
  float width  = mainWindow.getSize().x;
  float height = mainWindow.getSize().y;

  penOptionsPanel_ = tgui::Panel::create();
  penOptionsPanel_->setSize(width * 0.76, height * 0.062);
  penOptionsPanel_->setPosition(width * 0.22, height * 0.05);
  penOptionsPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  penOptionsPanel_->getRenderer()->setBorders({1});
  penOptionsPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  penOptionsPanel_->getRenderer()->setRoundedBorderRadius(8);
  penOptionsPanel_->setVisible(false);
  gui.add(penOptionsPanel_);

  // --- TAILLE ---
  auto sizeInputBox = tgui::EditBox::create();
  sizeInputBox->setSize(width * 0.06, height * 0.04);
  sizeInputBox->setPosition(width * 0.008, height * 0.011);
  sizeInputBox->setDefaultText("1");
  sizeInputBox->setInputValidator("[0-9]*");
  sizeInputBox->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  sizeInputBox->getRenderer()->setTextColor(tgui::Color::White);
  sizeInputBox->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
  sizeInputBox->getRenderer()->setBorders({1});
  sizeInputBox->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  sizeInputBox->onTextChange([this, sizeInputBox]() {
    if (sizeInputBox->getText().empty()) return;
    unsigned int size = static_cast<unsigned int>(std::stoi(sizeInputBox->getText().toStdString()));
    if (size < 1) size = 1;
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setSize(size, size);
  });
  penOptionsPanel_->add(sizeInputBox);

  // --- FORMES ---
  


  auto squareButton  = tgui::Button::create("Carré");
  auto diamondButton = tgui::Button::create("Diamant");
  auto circleButton  = tgui::Button::create("Cercle");

  squareButton->setSize(width * 0.065, height * 0.04);
  squareButton->setPosition(width * 0.1, height * 0.011);
  squareButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
  squareButton->getRenderer()->setTextColor(tgui::Color::White);
  squareButton->getRenderer()->setBorders({0});
  squareButton->getRenderer()->setRoundedBorderRadius(4);
  squareButton->onPress([this, squareButton, diamondButton, circleButton]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setShape(SQUARE);
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  });
  penOptionsPanel_->add(squareButton);
  squareButton->setTextSize(12);

  diamondButton->setSize(width * 0.065, height * 0.04);
  diamondButton->setPosition(width * 0.172, height * 0.011);
  diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  diamondButton->getRenderer()->setTextColor(tgui::Color::White);
  diamondButton->getRenderer()->setBorders({0});
  diamondButton->getRenderer()->setRoundedBorderRadius(4);
  diamondButton->onPress([this, squareButton, diamondButton, circleButton]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setShape(DIAMOND);
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  });
  penOptionsPanel_->add(diamondButton);
  diamondButton->setTextSize(12);

  circleButton->setSize(width * 0.065, height * 0.04);
  circleButton->setPosition(width * 0.244, height * 0.011);
  circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  circleButton->getRenderer()->setTextColor(tgui::Color::White);
  circleButton->getRenderer()->setBorders({0});
  circleButton->getRenderer()->setRoundedBorderRadius(4);
  circleButton->onPress([this, squareButton, diamondButton, circleButton]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setShape(CIRCLE);
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
  });
  penOptionsPanel_->add(circleButton);
  circleButton->setTextSize(12);

  // --- COULEURS PRÉDÉFINIES ---
  auto blackButton  = tgui::Button::create();
  auto whiteButton  = tgui::Button::create();
  auto greenButton  = tgui::Button::create();
  auto blueButton   = tgui::Button::create();
  auto yellowButton = tgui::Button::create();

  blackButton->setSize(width * 0.033, height * 0.04);
  blackButton->setPosition(width * 0.325, height * 0.011);
  blackButton->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0));
  blackButton->getRenderer()->setBackgroundColorHover(tgui::Color(0, 0, 0));
  blackButton->getRenderer()->setBorders({2});
  blackButton->getRenderer()->setBorderColor(tgui::Color(200, 220, 255));
  blackButton->getRenderer()->setRoundedBorderRadius(3);
  blackButton->onPress([this, blackButton, whiteButton, greenButton, blueButton, yellowButton]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setColor(sf::Color::Black);
    blackButton->getRenderer()->setBorders({2});
    blackButton->getRenderer()->setBorderColor(tgui::Color(200, 220, 255));
    whiteButton->getRenderer()->setBorders({0});
    greenButton->getRenderer()->setBorders({0});
    blueButton->getRenderer()->setBorders({0});
    yellowButton->getRenderer()->setBorders({0});
  });
  penOptionsPanel_->add(blackButton);

  whiteButton->setSize(width * 0.033, height * 0.04);
  whiteButton->setPosition(width * 0.363, height * 0.011);
  whiteButton->getRenderer()->setBackgroundColor(tgui::Color(255, 255, 255));
  whiteButton->getRenderer()->setBackgroundColorHover(tgui::Color(255, 255, 255));
  whiteButton->getRenderer()->setBorders({0});
  whiteButton->getRenderer()->setRoundedBorderRadius(3);
  whiteButton->onPress([this, blackButton, whiteButton, greenButton, blueButton, yellowButton]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setColor(sf::Color::White);
    blackButton->getRenderer()->setBorders({0});
    whiteButton->getRenderer()->setBorders({2});
    whiteButton->getRenderer()->setBorderColor(tgui::Color(200, 220, 255));
    greenButton->getRenderer()->setBorders({0});
    blueButton->getRenderer()->setBorders({0});
    yellowButton->getRenderer()->setBorders({0});
  });
  penOptionsPanel_->add(whiteButton);

  greenButton->setSize(width * 0.033, height * 0.04);
  greenButton->setPosition(width * 0.401, height * 0.011);
  greenButton->getRenderer()->setBackgroundColor(tgui::Color(50, 200, 80));
  greenButton->getRenderer()->setBackgroundColorHover(tgui::Color(50, 200, 80));
  greenButton->getRenderer()->setBorders({0});
  greenButton->getRenderer()->setRoundedBorderRadius(3);
  greenButton->onPress([this, blackButton, whiteButton, greenButton, blueButton, yellowButton]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setColor(sf::Color(50, 200, 80));
    blackButton->getRenderer()->setBorders({0});
    whiteButton->getRenderer()->setBorders({0});
    greenButton->getRenderer()->setBorders({2});
    greenButton->getRenderer()->setBorderColor(tgui::Color(200, 220, 255));
    blueButton->getRenderer()->setBorders({0});
    yellowButton->getRenderer()->setBorders({0});
  });
  penOptionsPanel_->add(greenButton);

  blueButton->setSize(width * 0.033, height * 0.04);
  blueButton->setPosition(width * 0.439, height * 0.011);
  blueButton->getRenderer()->setBackgroundColor(tgui::Color(30, 100, 255));
  blueButton->getRenderer()->setBackgroundColorHover(tgui::Color(30, 100, 255));
  blueButton->getRenderer()->setBorders({0});
  blueButton->getRenderer()->setRoundedBorderRadius(3);
  blueButton->onPress([this, blackButton, whiteButton, greenButton, blueButton, yellowButton]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setColor(sf::Color(30, 100, 255));
    blackButton->getRenderer()->setBorders({0});
    whiteButton->getRenderer()->setBorders({0});
    greenButton->getRenderer()->setBorders({0});
    blueButton->getRenderer()->setBorders({2});
    blueButton->getRenderer()->setBorderColor(tgui::Color(200, 220, 255));
    yellowButton->getRenderer()->setBorders({0});
  });
  penOptionsPanel_->add(blueButton);

  yellowButton->setSize(width * 0.033, height * 0.04);
  yellowButton->setPosition(width * 0.477, height * 0.011);
  yellowButton->getRenderer()->setBackgroundColor(tgui::Color(255, 220, 0));
  yellowButton->getRenderer()->setBackgroundColorHover(tgui::Color(255, 220, 0));
  yellowButton->getRenderer()->setBorders({0});
  yellowButton->getRenderer()->setRoundedBorderRadius(3);
  yellowButton->onPress([this, blackButton, whiteButton, greenButton, blueButton, yellowButton]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setColor(sf::Color(255, 220, 0));
    blackButton->getRenderer()->setBorders({0});
    whiteButton->getRenderer()->setBorders({0});
    greenButton->getRenderer()->setBorders({0});
    blueButton->getRenderer()->setBorders({0});
    yellowButton->getRenderer()->setBorders({2});
    yellowButton->getRenderer()->setBorderColor(tgui::Color(200, 220, 255));
  });
  penOptionsPanel_->add(yellowButton);

  // --- RGB PERSONNALISÉ ---
  auto rLabel = tgui::Label::create("R");
  rLabel->setSize(width * 0.014, height * 0.04);
  rLabel->setPosition(width * 0.522, height * 0.011);
  rLabel->getRenderer()->setTextColor(tgui::Color::White);
  penOptionsPanel_->add(rLabel);

  auto rInput = tgui::EditBox::create();
  rInput->setSize(width * 0.038, height * 0.04);
  rInput->setPosition(width * 0.544, height * 0.011);
  rInput->setText("0");
  rInput->setInputValidator("[0-9]*");
  rInput->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  rInput->getRenderer()->setTextColor(tgui::Color::White);
  rInput->getRenderer()->setBorders({1});
  rInput->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  penOptionsPanel_->add(rInput);

  auto gLabel = tgui::Label::create("G");
  gLabel->setSize(width * 0.014, height * 0.04);
  gLabel->setPosition(width * 0.59, height * 0.011);
  gLabel->getRenderer()->setTextColor(tgui::Color::White);
  penOptionsPanel_->add(gLabel);

  auto gInput = tgui::EditBox::create();
  gInput->setSize(width * 0.038, height * 0.04);
  gInput->setPosition(width * 0.608, height * 0.011);
  gInput->setText("0");
  gInput->setInputValidator("[0-9]*");
  gInput->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  gInput->getRenderer()->setTextColor(tgui::Color::White);
  gInput->getRenderer()->setBorders({1});
  gInput->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  penOptionsPanel_->add(gInput);

  auto bLabel = tgui::Label::create("B");
  bLabel->setSize(width * 0.014, height * 0.04);
  bLabel->setPosition(width * 0.654, height * 0.011);
  bLabel->getRenderer()->setTextColor(tgui::Color::White);
  penOptionsPanel_->add(bLabel);

  auto bInput = tgui::EditBox::create();
  bInput->setSize(width * 0.038, height * 0.04);
  bInput->setPosition(width * 0.672, height * 0.011);
  bInput->setText("0");
  bInput->setInputValidator("[0-9]*");
  bInput->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  bInput->getRenderer()->setTextColor(tgui::Color::White);
  bInput->getRenderer()->setBorders({1});
  bInput->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  penOptionsPanel_->add(bInput);

  auto applyRgbBtn = tgui::Button::create("OK");
  applyRgbBtn->setSize(width * 0.033, height * 0.04);
  applyRgbBtn->setPosition(width * 0.716, height * 0.011);
  applyRgbBtn->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
  applyRgbBtn->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
  applyRgbBtn->getRenderer()->setTextColor(tgui::Color::White);
  applyRgbBtn->getRenderer()->setBorders({0});
  applyRgbBtn->getRenderer()->setRoundedBorderRadius(4);
  applyRgbBtn->onPress([this, rInput, gInput, bInput]() {
    int r = rInput->getText().empty() ? 0 : std::min(255, std::stoi(rInput->getText().toStdString()));
    int g = gInput->getText().empty() ? 0 : std::min(255, std::stoi(gInput->getText().toStdString()));
    int b = bInput->getText().empty() ? 0 : std::min(255, std::stoi(bInput->getText().toStdString()));
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setColor(sf::Color(r, g, b));
  });
  penOptionsPanel_->add(applyRgbBtn);
}

void Window::initEraserOptions() {
  float width  = mainWindow.getSize().x;
  float height = mainWindow.getSize().y;

  eraserOptionsPanel_ = tgui::Panel::create();
  eraserOptionsPanel_->setSize(width * 0.35, height * 0.062);
  eraserOptionsPanel_->setPosition(width * 0.35, height * 0.05);
  eraserOptionsPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  eraserOptionsPanel_->getRenderer()->setBorders({1});
  eraserOptionsPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  eraserOptionsPanel_->getRenderer()->setRoundedBorderRadius(8);
  eraserOptionsPanel_->setVisible(false);
  gui.add(eraserOptionsPanel_);

  // --- TAILLE ---
  auto sizeInputBox = tgui::EditBox::create();
  sizeInputBox->setSize(width * 0.06, height * 0.04);
  sizeInputBox->setPosition(width * 0.008, height * 0.011);
  sizeInputBox->setDefaultText("1");
  sizeInputBox->setInputValidator("[0-9]*");
  sizeInputBox->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  sizeInputBox->getRenderer()->setTextColor(tgui::Color::White);
  sizeInputBox->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
  sizeInputBox->getRenderer()->setBorders({1});
  sizeInputBox->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  sizeInputBox->onTextChange([this, sizeInputBox]() {
    if (sizeInputBox->getText().empty()) return;
    unsigned int size = static_cast<unsigned int>(std::stoi(sizeInputBox->getText().toStdString()));
    if (size < 1) size = 1;
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setSize(size, size);
  });
  eraserOptionsPanel_->add(sizeInputBox);

  // --- FORMES ---
  auto squareButton  = tgui::Button::create("Carré");
  auto diamondButton = tgui::Button::create("Diamant");
  auto circleButton  = tgui::Button::create("Cercle");

  squareButton->setSize(width * 0.065, height * 0.04);
  squareButton->setPosition(width * 0.1, height * 0.011);
  squareButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
  squareButton->getRenderer()->setTextColor(tgui::Color::White);
  squareButton->getRenderer()->setBorders({0});
  squareButton->getRenderer()->setRoundedBorderRadius(4);
  squareButton->onPress([this, squareButton, diamondButton, circleButton]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setShape(SQUARE);
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  });
  eraserOptionsPanel_->add(squareButton);
  squareButton->setTextSize(12);

  diamondButton->setSize(width * 0.065, height * 0.04);
  diamondButton->setPosition(width * 0.172, height * 0.011);
  diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  diamondButton->getRenderer()->setTextColor(tgui::Color::White);
  diamondButton->getRenderer()->setBorders({0});
  diamondButton->getRenderer()->setRoundedBorderRadius(4);
  diamondButton->onPress([this, squareButton, diamondButton, circleButton]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setShape(DIAMOND);
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  });
  eraserOptionsPanel_->add(diamondButton);
  diamondButton->setTextSize(12);

  circleButton->setSize(width * 0.065, height * 0.04);
  circleButton->setPosition(width * 0.244, height * 0.011);
  circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  circleButton->getRenderer()->setTextColor(tgui::Color::White);
  circleButton->getRenderer()->setBorders({0});
  circleButton->getRenderer()->setRoundedBorderRadius(4);
  circleButton->onPress([this, squareButton, diamondButton, circleButton]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setShape(CIRCLE);
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
  });
  eraserOptionsPanel_->add(circleButton);
  circleButton->setTextSize(12);
}