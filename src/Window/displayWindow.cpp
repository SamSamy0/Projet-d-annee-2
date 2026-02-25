#include "displayWindow.hpp"
#include "../project/project.hpp"
#include "../project/tool.hpp"
#include "../server/clientnetwork.hpp"
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

void Window::updateTextSize() {
  const float windowHeight = gui.getView().getRect().height;
  gui.setTextSize(static_cast<unsigned int>(0.03 * windowHeight));
}
void Window::closePopup() {
  auto popup = gui.get("popup");
  gui.remove(popup);
  if (activeMoreButton) {
    activeMoreButton->getRenderer()->setBackgroundColor(
        tgui::Color::Transparent);
    activeMoreButton = nullptr;
  }
}
void Window::processEvents() {
  while (const std::optional event = mainWindow.pollEvent()) {
    gui.handleEvent(*event);
    // Close the Window
    if (event->is<sf::Event::Closed>())
      mainWindow.close();

    // Echap to escape
    if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      if (keyPressed->code == sf::Keyboard::Key::Escape)
        mainWindow.close();
    }
    if (const auto *mouseClick =
            event->getIf<sf::Event::MouseButtonPressed>()) {
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
        auto backgroundPanel =
            std::dynamic_pointer_cast<tgui::Panel>(background);
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

    // Dans un projet
    if (state == projectState::GAME && project) {
      project->getMap()->detectMovement();

      // TOOLS
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
      if (const auto *wheelEvent =
              event->getIf<sf::Event::MouseWheelScrolled>()) {
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
  }
}

void Window::setState(projectState newState) {
  this->state = newState;
  this->initWidget();
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
void Window::run() {
  while (mainWindow.isOpen()) {
    processEvents();
    mainWindow.clear(sf::Color(35, 35, 40));

    if (Window::state == projectState::LOGIN ||
        Window::state == projectState::MENU) {
    } else if (Window::state == projectState::GAME && project) {
      // Display Game
      project->display();
    }
    gui.draw();
    mainWindow.display();
  }
}
