#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Text.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <iostream>

int main() {
  sf::RenderWindow window(sf::VideoMode({1200, 800}), "Test");

  tgui::Gui gui(window);

  auto button = tgui::Button::create("Home");
  button->setSize(100, 40);
  button->setPosition(20, 20);

  button->getRenderer()->setBackgroundColor(tgui::Color(26, 188, 187));
  button->getRenderer()->setBackgroundColorHover(tgui::Color(70, 200, 200));

  button->onPress([]() { std::cout << "Clicked\n"; });

  gui.add(button);

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();

      gui.handleEvent(*event);
    }

    window.clear();
    gui.draw();
    window.display();
  }
}
