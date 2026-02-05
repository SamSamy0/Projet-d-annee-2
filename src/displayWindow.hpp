#ifndef DISPLAYWINDOW_HPP
#define DISPLAYWINDOW_HPP
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
class Window {
 private:
  sf::RenderWindow mainWindow;
  tgui::Gui gui;
  void initWidget();
  void updateTextSize();
  void processEvents();

 public:
  Window();
  void run();
};
#endif
