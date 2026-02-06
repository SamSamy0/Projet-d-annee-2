#ifndef DISPLAYWINDOW_HPP
#define DISPLAYWINDOW_HPP
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <unordered_map>
class Window {
private:
  std::unordered_map<std::string, std::string> DB;
  bool isLoggedIn;
  sf::RenderWindow mainWindow;
  tgui::Gui gui;
  void initWidget();
  void updateTextSize();
  void processEvents();
  void login(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  bool signIn(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  void menuWindow();
  bool checkLogin();

public:
  Window();
  void run();
};
#endif
