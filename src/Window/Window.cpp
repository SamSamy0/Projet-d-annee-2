#include "Window.hpp"
#include "../project/project.hpp"
#include "../project/tool.hpp"
#include "../client/clientnetwork.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Layout.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Text.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/ListBox.hpp>
#include <TGUI/Widgets/ScrollablePanel.hpp>
#include <TGUI/Widgets/Scrollbar.hpp>
#include <TGUI/Widgets/TextArea.hpp>
#include <iostream>
#include <memory>
#include <vector>

void Window::initWidget() {
  gui.removeAllWidgets();
  updateTextSize();
  gui.onViewChange([this] { updateTextSize(); });

  if (this->state == projectState::LOGIN) {
    Window::loginWidget();
  } else if (this->state == projectState::MENU) {
    Window::initMenuWidget();
  } else if (this->state == projectState::GAME) {
    initGameWidget();
  }
}

void Window::processEvents() {
  while (const std::optional event = mainWindow.pollEvent()) {
    gui.handleEvent(*event);
    // Close the Window
    handleWindowEvents(event);
    handlePopupEvents(event);

    // Dans un projet
    if (state == projectState::GAME && project) {
      handleGameEvents(event);

      // TOOLS
    }
  }
}

void Window::handleWindowEvents(const std::optional<sf::Event> &event) {
  if (event->is<sf::Event::Closed>())
    mainWindow.close();

  // Echap to escape
  if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
    if (keyPressed->code == sf::Keyboard::Key::Escape)
      mainWindow.close();
  }
}
void Window::handlePopupEvents(const std::optional<sf::Event> &event) {
  if (const auto *mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
    auto popup = gui.get("popup");
    auto background = gui.get("background");
    auto btnMore = gui.get("BtnMore");
    // If popup exists
    if (popup) {
      // Gets position of where menu pops
      sf::Vector2f clickPos(mouseClick->position.x, mouseClick->position.y);
      // If click outside the menu, then remove it
      if (!popup->isMouseOnWidget(clickPos)) {
        closePopup();
      }
    }
    if (background) {
      // Cast to Panel to use (->get("dataPanel"))
      auto backgroundPanel = std::dynamic_pointer_cast<tgui::Panel>(background);
      auto dataPanel = backgroundPanel->get("dataPanel");
      sf::Vector2f clickPos(mouseClick->position.x, mouseClick->position.y);
      if (dataPanel && !dataPanel->isMouseOnWidget(clickPos)) {
        gui.remove(background);
      }
    }
  }
  if (event->getIf<sf::Event::MouseWheelScrolled>()) {
    auto popup = gui.get("popup");
    if (popup) {
      gui.remove(popup);
    }
  }
}

void Window::setState(projectState newState) {
  this->state = newState;
  this->initWidget();
}

void Window::updateTextSize() {
  const float windowHeight = gui.getView().getRect().height;
  gui.setTextSize(static_cast<unsigned int>(0.03 * windowHeight));
}

Window::Window(ClientNetworkManager &manager)
    : mainWindow(sf::VideoMode::getDesktopMode(), "Game name",
                 sf::State::Fullscreen),
      gui{mainWindow}, manager{manager}, project{nullptr} {
  initWidget();
}

void Window::setProjectList(std::vector<ProjectData> newlist) {
  projectList = newlist;
}
bool Window::isOpen() const { return mainWindow.isOpen(); }

void Window::run() {
  processEvents();
  mainWindow.clear(sf::Color(35, 35, 40));

  switch (Window::state){
    case projectState::LOGIN:
      if (isLoggedIn){
        state = projectState::MENU;
      }
      gui.draw();
      mainWindow.display();
    
    case projectState::MENU:
      gui.draw();
      mainWindow.display();
    case projectState::GAME:
      ;
  };
  
  /*if (Window::state == projectState::GAME && project) {
    // Display Game
    project->display();
  }
  gui.draw();
  mainWindow.display();*/
}

void Window::setLogIn(){
  isLoggedIn = true;
}

// Pour une utilisation sans serveur
// mais dans ce cas, il faut retirer la boucle dans client.cpp
//  void Window::run() {
//    while (mainWindow.isOpen()) {
//      processEvents();
//      mainWindow.clear(sf::Color(35, 35, 40));
//
//      if (Window::state == projectState::GAME && project) {
//        // Display Game
//        project->display();
//      }
//      gui.draw();
//      mainWindow.display();
//    }
//  }
