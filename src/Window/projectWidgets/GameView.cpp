#include "GameView.hpp"
#include "../../project/Chat/userMessage.hpp"
#include "../../project/Tool/pixelbrush.hpp"
#include "../../project/Tool/pixelshift.hpp"
#include "../../project/Tool/spriteselection.hpp"
#include "../MenuView.hpp"
#include <qnamespace.h>

GameView::GameView(Application &app)
    : View(app), project(app_.getProject().get()),
      currentUser(app_.getUser()) {};

void GameView::init() {
  app_.getGui().removeAllWidgets();

  initToolbar();
  initPenOptions();
  initPenSpriteOptions();
  initEraserOptions();
  initEraserSpriteOptions();
  initSpriteBrushOptions();
  initAutoFillOptions();
  initLayerPanel();  initChatWidget();
  initMinimap();
  refreshChat();
}

void GameView::displayMemberList() {
  auto &gui = app_.getGui();
  auto &manager = app_.getNetwork();

  if (gui.get("memberListPopup")) {
    gui.remove(gui.get("memberListPopup"));
  }

  // If owner is alone, he can leave project instantly
  if (Transferring && allUsers_.size() == 1 &&
      allUsers_[0].userId == currentUser.getId()) {
    app_.getNetwork().leaveProject(project->getId());
    app_.getNetwork().getProjectList();
    app_.changeView(std::make_unique<MenuView>(app_));
    return;
  }

  auto parent = tgui::Panel::create();
  parent->setSize("40%", "60%");
  parent->setPosition("30%", "20%");
  parent->getRenderer()->setBackgroundColor({40, 40, 40, 240});
  parent->getRenderer()->setRoundedBorderRadius(10);
  gui.add(parent, "memberListPopup");

  // Title

  auto title =
      tgui::Label::create(Transferring ? "Choisissez un nouveau propriétaire"
                                       : "Membres du projet");
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
      manager.changeRole(userId, project->getId(), 2);
      app_.getNetwork().leaveProject(project->getId());
      app_.getNetwork().getProjectList();
      app_.changeView(std::make_unique<MenuView>(app_));
    });

    std::string nomRole = (allUsers_[i].role == 1) ? "Editeur" : "Spectateur";
    if (allUsers_[i].role == 2)
      nomRole = "Propriétaire";

    auto labelRole = tgui::Label::create(nomRole);
    if (Transferring) {
      labelRole->setPosition("70%", "center");
      labelRole->setTextSize(14);
      labelRole->getRenderer()->setTextColor(sf::Color(140, 140, 160));
      row->add(labelRole);
      row->add(clickable);

    } else {
      // Adding more button
      if (project->getRole() == 2) {
        labelRole->setPosition("62.5%", "center");
        auto btn = tgui::Button::create("•••");
        btn->setSize(80, 35);
        btn->setPosition("100% -90", "center + 10");
        btn->getRenderer()->setBackgroundColor(sf::Color::Transparent);
        btn->getRenderer()->setBackgroundColorHover(sf::Color(55, 55, 70));
        btn->getRenderer()->setTextColor(sf::Color(140, 140, 160));
        btn->getRenderer()->setBorders(0);
        btn->getRenderer()->setRoundedBorderRadius(6);
        row->add(labelRole);
        row->add(btn, "BtnMore");
        btn->onPress(&GameView::showUserManagment, this, btn, i);
        btn->setTextSize(20);
      } else {
        labelRole->setPosition("70%", "center");
        row->add(labelRole);
      }
    }
  }
}

void GameView::showUserManagment(tgui::Button::Ptr toHover, int place) {
  auto &gui = app_.getGui();
  auto &manager = app_.getNetwork();
  activeMoreButton = toHover;
  sf::Vector2f btnPos = toHover->getAbsolutePosition();
  activeMoreButton->getRenderer()->setBackgroundColor(sf::Color(55, 55, 70));
  auto menu = tgui::ListBox::create();
  menu->getScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  menu->getRenderer()->setBorders(2);
  menu->getRenderer()->setBorderColor(sf::Color::White);
  menu->getRenderer()->setBackgroundColor(sf::Color(40, 40, 52));
  menu->getRenderer()->setTextColor(tgui::Color::White);
  menu->addItem("Promouvoir");
  menu->addItem("Retrograder");
  menu->addItem("Ejecter");
  // }
  menu->addItem("Propriétaire");
  // menu->addItem("Quitter");
  float menuHeight = menu->getItemCount() * 45;
  menu->setSize(150, menuHeight);
  menu->setItemHeight(45);

  menu->setPosition(btnPos.x - 150, btnPos.y);

  gui.add(menu, "popup");
  menu->setTextSize(20);
  menu->onItemSelect(
      [this, menu, place, &manager, &gui](const tgui::String &item) {
        int8_t role = project->getRole();
        if (item == "Promouvoir") {
          // Spector -> Editor
          if (allUsers_[place].role == 0) {
            manager.changeRole(allUsers_[place].userId, project->getId(), 1);
          } else if (role == 1) {
            // WARNING YOU ARE LOSING YOUR OWNER STATUS
          }

        } else if (item == "Retrograder") {
          if (allUsers_[place].role == 1) {
            manager.changeRole(allUsers_[place].userId, project->getId(), 0);
          } else if (role == 1) {
            // WARNING YOU CAN'T DOWNGRADE A SPECTATOR
          }
        } else if (item == "Ejecter") {
          manager.kickUser(allUsers_[place].userId, project->getId());
          displayMemberList();

        } else if (item == "Propriétaire") {
          // We find the owner position
          for (int i = 0; i < (int)allUsers_.size(); i++) {
            if (allUsers_[i].pseudo == currentUser.getUser()) {
              // We downgrade owner to editor
              manager.changeRole(allUsers_[i].userId, project->getId(), 1);
            }
          }
          // Change target role to owner
          manager.changeRole(allUsers_[place].userId, project->getId(), 2);
        }

        activeMoreButton->getRenderer()->setBackgroundColor(
            tgui::Color::Transparent);
        gui.remove(menu);
      });
}

void GameView::clearMemberList() { allUsers_.clear(); }

void GameView::render() {
  if (project) drawMinimap();
}

void GameView::handleEvents(const sf::Event &event) {
  auto &mainWindow = app_.getWindow();
  auto &gui = app_.getGui();

  // On évite de se déplacer en écrivant
  auto focusedWidget = gui.getFocusedLeaf();
  bool textInputFocused = chatInput_->isFocused() ||
      (focusedWidget && std::dynamic_pointer_cast<tgui::EditBox>(focusedWidget) != nullptr);
  if (!textInputFocused)
    project->getMap()->detectMovement();
  
    // ON PRESS
  if (const auto *mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
    // On menu
    auto popup = gui.get("popup");
    auto exportPopup = gui.get("exportPopup");
    // If popup exists
    if (popup) {
      // Gets position of where menu pops
      sf::Vector2f clickPos(mousePressed->position.x, mousePressed->position.y);
      // If click outside the menu, then remove it
      if (!popup->isMouseOnWidget(clickPos)) {
        gui.remove(popup);
        if (activeMoreButton) {
          activeMoreButton->getRenderer()->setBackgroundColor(
              tgui::Color::Transparent);
          activeMoreButton = nullptr;
        }
      }
    }
    if (exportPopup) {
      // Gets position of where menu pops
      sf::Vector2f clickPos(mousePressed->position.x, mousePressed->position.y);
      // If click outside the menu, then remove it
      if (!exportPopup->isMouseOnWidget(clickPos)) {
        gui.remove(exportPopup);
      }
    }

    // On map
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
        float step = std::min(mapSize.x, mapSize.y) / 2000.0f;

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
  if (event.is<sf::Event::Resized>()) refreshChat();

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Delete)) {
    std::shared_ptr<Tool> tool = project->getToolBar().getSelectedTool();
    if (tool->getType() == SPRITESELECTION)
      static_pointer_cast<SpriteSelection>(tool)->erase();
  }
}

void GameView::updateMemberRole(uint targetId, int8_t newRole) {
  for (auto &i : allUsers_) {
    if (i.userId == targetId) {
      i.role = newRole;
    }
  }

  // My role Changed
  if (currentUser.getId() == targetId) {
    project->setRole(newRole);
    app_.setCurrentProjRole(newRole);
    init();
  }
  auto &gui = app_.getGui();
  if (gui.get("memberListPopup")) {
    displayMemberList();
  }
}

void GameView::getAllUsers() {
  auto &manager = app_.getNetwork();
  manager.getUsersProjects(project->getId());
}
void GameView::setAllUsers(std::vector<MemberEntry> users) {
  allUsers_ = users;
  displayMemberList();
  Transferring = false;
}

void GameView::popupWarning(std::string motif) {
  auto &gui = app_.getGui();
  auto back = tgui::Panel::create();
  back->setSize("100%", "100%");
  back->getRenderer()->setBackgroundColor({0, 0, 0, 160});
  gui.add(back, "back");

  auto popup = tgui::Panel::create();
  popup->setSize("30%", "20%");
  popup->setPosition("35%", "40%");
  popup->getRenderer()->setBackgroundColor(tgui::Color(28, 28, 36));
  popup->getRenderer()->setBorders(1);
  popup->getRenderer()->setBorderColor(tgui::Color(200, 60, 60));
  popup->getRenderer()->setRoundedBorderRadius(12);
  back->add(popup);

  tgui::Label::Ptr icon;
  tgui::Label::Ptr msg;
  if (motif == "kick") {
    icon = tgui::Label::create("⚠");
     msg = tgui::Label::create("Vous avez été expulsé du projet");
  } else if (motif == "exportNatif") {
    icon = tgui::Label::create("⚠");
     msg = tgui::Label::create("Vous devez d'abord sauvegarder le projet");
  } else if (motif == "exportPngOK") {
    icon = tgui::Label::create("✔");
     msg = tgui::Label::create(
        "Votre image à été sauvegardé dans le dossier export_image");
    popup->getRenderer()->setBorderColor(tgui::Color::Green);
  } else if (motif == "exportPngKO") {
    icon = tgui::Label::create("⚠");
    msg =
        tgui::Label::create("L'export au format image n'a pas pu aboutir");
  }
  msg->setPosition("5%", "45%");
  msg->getRenderer()->setTextSize(20);
  msg->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
  msg->getRenderer()->setTextColor(tgui::Color(220, 220, 235));
  popup->add(msg);

    icon->setPosition("50%", "10%");
  icon->getRenderer()->setTextSize(40);
  if (motif == "exportPngKO")
    icon->getRenderer()->setTextColor(tgui::Color(200, 60, 60));
  else if(motif=="exportPngOK")
    icon->getRenderer()->setTextColor(tgui::Color::Green);
  popup->add(icon);


  auto okBtn = tgui::Button::create("OK");
  okBtn->setSize("40%", "20%");
  okBtn->setPosition("30%", "72%");
  okBtn->getRenderer()->setBackgroundColor(tgui::Color(99, 102, 241));
  okBtn->getRenderer()->setBackgroundColorHover(tgui::Color(118, 120, 255));
  okBtn->getRenderer()->setTextColor(tgui::Color::White);
  okBtn->getRenderer()->setBorders(0);
  okBtn->getRenderer()->setRoundedBorderRadius(8);
  popup->add(okBtn);

  okBtn->onPress([this, motif,&gui]() {
    gui.remove(gui.get("back"));
    if (motif == "kick"){
      app_.getNetwork().getProjectList();
      app_.changeView(std::make_unique<MenuView>(app_));
    }
  });
}
