#ifndef MENU_HPP
#define MENU_HPP
#include "Identifier.hpp"
#include "displayWindow.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Text.hpp>
#include <TGUI/Widgets/TextArea.hpp>

class MenuWin {
  sf::RenderWindow menuWindow;
  tgui::Gui MenuGui;
  // Maybe do inheritance because MenuWin has some same function as AuthWin
  // in displayWindow
  void menuWidget();
  void updateTextSize();
  void processEvents();

public:
  MenuWin();
  // Don't know if I do a run function
  void run();
};

#endif
