#include "../Window.hpp"
#include "../../project/Tool/pixelbrush.hpp"
#include "../../project/Tool/pixelshift.hpp"
#include "../../project/Chat/userMessage.hpp"

void Window::initGameWidget() {
  initToolbar();
  initLayerPanel();
  initChatWidget();
  initPenOptions();
  initEraserOptions();
  refreshChat();
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
      std::shared_ptr<Tool> tool = project->getToolBar().getSelectedTool();
      if (tool->getType() == PIXELBRUSH || tool->getType() == SPRITEBRUSH || tool->getType() == SPRITEERASER) {
        std::shared_ptr<Brush> brush = static_pointer_cast<Brush>(tool); 
        sf::Vector2u size = brush->getSize();
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

        if (sizex < 1) {
          sizex = 1;
        } else if (sizex > mapSize.x) {
          sizex = mapSize.x;
        }
        if (sizey < 1) {
          sizey = 1;
        } else if (sizey > mapSize.y) {
          sizey = mapSize.y;
        }

        size = sf::Vector2u(static_cast<unsigned int>(std::round(sizex)),
                            static_cast<unsigned
                            int>(std::round(sizey)));
        brush->setSize(size.x, size.y);
      }
    } else {
      if (!gui.getWidgetBelowMouseCursor(wheelEvent->position, true))
        project->getMap()->zooming(wheelEvent); // ZOOM
    }
  }
}



