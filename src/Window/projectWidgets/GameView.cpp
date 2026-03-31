#include "GameView.hpp"
#include "../../project/Chat/userMessage.hpp"
#include "../../project/Tool/pixelbrush.hpp"
#include "../../project/Tool/pixelshift.hpp"
#include "../MenuView.hpp"

GameView::GameView(Application &app)
    : View(app), project(app_.getProject().get()),
      currentUser(app_.getUser()) {};

void GameView::init() {
  app_.getGui().removeAllWidgets();

  // If not spectator
  if (project->getRole() != 0) {
    initToolbar();
    initPenOptions();
    initPenSpriteOptions();
    initEraserOptions();
    initEraserSpriteOptions();
    initSpriteBrushOptions();
  }
  initLayerPanel();
  initChatWidget();
  initMinimap();
  refreshChat();
}

void GameView::displayMemberList() {
  auto &gui = app_.getGui();
  auto &manager = app_.getNetwork();

  if (gui.get("memberListPopup")) {
    gui.remove(gui.get("memberListPopup"));
  }

  auto parent = tgui::Panel::create();
  parent->setSize("40%", "60%");
  parent->setPosition("30%", "20%");
  parent->getRenderer()->setBackgroundColor({40, 40, 40, 240});
  parent->getRenderer()->setRoundedBorderRadius(10);
  gui.add(parent, "memberListPopup");

  // Title
  auto title = tgui::Label::create("Choisissez un nouveau propriétaire");
  title->setPosition("center", "5%");
  title->setTextSize(24);
  title->getRenderer()->setTextColor(sf::Color::White);
  parent->add(title);

  auto exitB = tgui::Button::create("✕");
  exitB->setSize(30, 30);
  exitB->setPosition("100% - 35", "5");
  exitB->getRenderer()->setBackgroundColor(sf::Color::Transparent);
  exitB->getRenderer()->setTextColor(sf::Color::White);
  exitB->getRenderer()->setBackgroundColorHover(sf::Color(255, 100, 100));
  parent->add(exitB);

  exitB->onPress(
      [this]() { app_.getGui().remove(app_.getGui().get("memberListPopup")); });

  auto panel = tgui::ScrollablePanel::create();
  panel->setPosition("5%", "18%");
  panel->setSize("90%", "75%");
  panel->getRenderer()->setBackgroundColor(sf::Color::Transparent);
  panel->getRenderer()->setBorders(0);
  panel->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Automatic);
  parent->add(panel);

  for (int i = 0; i < (int)allUsers_.size(); i++) {
    if (allUsers_[i].pseudo == currentUser.getUser()) {
      continue;
    }
    auto row = tgui::Panel::create();
    row->setSize("96%", 56);
    row->setPosition("2%", i * 64);
    row->getRenderer()->setBackgroundColor(sf::Color(32, 32, 40));
    row->getRenderer()->setRoundedBorderRadius(8);
    panel->add(row);

    // Pseudo
    std::string pseudo = allUsers_[i].pseudo;
    int8_t userId = allUsers_[i].userId;
    auto label = tgui::Label::create(pseudo);
    label->setPosition(20, "center");
    label->setTextSize(18);
    label->getRenderer()->setTextColor(sf::Color(220, 220, 235));
    row->add(label);

    // Clickable
    auto clickable = tgui::Button::create();
    clickable->setSize("100%", "100%");
    clickable->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    clickable->getRenderer()->setBackgroundColorHover(
        tgui::Color(255, 255, 255, 20));
    clickable->getRenderer()->setRoundedBorderRadius(8);
    Project *proj = project;
    clickable->onPress([this, pseudo, userId, proj, &manager] {
      std::cout << "click on " << pseudo;
      std::cout << " id is " << static_cast<int>(userId) << std::endl;
      manager.changeRole(userId, project->getId(), 2);
      // BUG: Mauvais id du user
      app_.getNetwork().leaveProject(project->getId());
      app_.getNetwork().getProjectList();
      app_.changeView(std::make_unique<MenuView>(app_));

      // Get user by pseudo
      // Ask server to change owner (and to delete current Owner)
    });

    std::string nomRole = (allUsers_[i].role == 1) ? "Editeur" : "Spectateur";
    if (allUsers_[i].role == 2)
      nomRole = "Propriétaire";

    auto labelRole = tgui::Label::create(nomRole);
    labelRole->setPosition("100% - 300", "center");
    labelRole->setTextSize(14);
    labelRole->getRenderer()->setTextColor(sf::Color(140, 140, 160));
    row->add(labelRole);
    row->add(clickable);
  }
}
void GameView::clearMemberList() { allUsers_.clear(); }

void GameView::render() {
  if (project)
    drawMinimap();
}

void GameView::handleEvents(const sf::Event &event) {
  auto &mainWindow = app_.getWindow();
  auto &gui = app_.getGui();
  if (!chatInput_->isFocused())
    project->getMap()->detectMovement();
  // ON PRESS
  if (const auto *mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
    if (mousePressed->button == sf::Mouse::Button::Left) {
      sf::Vector2i mousePos = mousePressed->position; // mouse position
      if (!gui.getWidgetBelowMouseCursor(mousePos, true)) {
        sf::Vector2f pos =
            mainWindow.mapPixelToCoords(mousePos, project->getView());
        sf::Vector2i mapPos(static_cast<int>(pos.x), static_cast<int>(pos.y));
        project->getToolBar().getSelectedTool()->onPress(mapPos);
      }
    }
  }

  // ON RELEASE
  if (auto mouseEvent = event.getIf<sf::Event::MouseButtonReleased>()) {
    if (mouseEvent->button == sf::Mouse::Button::Left) {
      if (!gui.getWidgetBelowMouseCursor(mouseEvent->position, true)) {
        project->getToolBar().getSelectedTool()->onRelease();
      }
    }
  }

  // ON DRAG
  if (const auto *mouseMoved = event.getIf<sf::Event::MouseMoved>()) {
    sf::Vector2i mousePos = mouseMoved->position; // mouse position
    if (!gui.getWidgetBelowMouseCursor(mousePos, true)) {
      sf::Vector2f pos =
          mainWindow.mapPixelToCoords(mousePos, project->getView());
      sf::Vector2i mapPos(static_cast<int>(pos.x), static_cast<int>(pos.y));
      project->getToolBar().getSelectedTool()->onDrag(mapPos);
    }
  }

  // SET SIZE
  if (const auto *wheelEvent = event.getIf<sf::Event::MouseWheelScrolled>()) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
      std::shared_ptr<Tool> tool = project->getToolBar().getSelectedTool();

      if (tool->getType() == PIXELBRUSH || tool->getType() == SPRITEBRUSH ||
          tool->getType() == SPRITEERASER) {
        std::shared_ptr<Brush> brush = static_pointer_cast<Brush>(tool);
        sf::Vector2f size = brush->getSize();
        float sizex = size.x;
        float sizey = size.y;
        sf::Vector2u mapSize = project->getMap()->getSize();
        float step = std::min(mapSize.x, mapSize.y) / 200.0f;

        if (wheelEvent->delta > 0) {
          sizex += step;
          sizey += step;
        } else if (wheelEvent->delta < 0) {
          sizex -= step;
          sizey -= step;
        }

        brush->setSize(sizex, sizey);
      }
    } else {
      if (!gui.getWidgetBelowMouseCursor(wheelEvent->position, true))
        project->getMap()->zooming(wheelEvent); // ZOOM
    }
  }
}

void GameView::getAllUsers() {
  auto &manager = app_.getNetwork();
  manager.getUsersProjects(project->getId());
}
void GameView::setAllUsers(std::vector<MemberEntry> users) {
  allUsers_ = users;
  displayMemberList();
}
