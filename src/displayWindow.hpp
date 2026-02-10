#ifndef DISPLAYWINDOW_HPP
#define DISPLAYWINDOW_HPP
#include "Identifier.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <vector>

enum class projectState { LOGIN, MENU, GAME };
class AuthWindow {
private:
  projectState state = projectState::LOGIN;
  // Temporary
  std::vector<Identifier> DB;
  // Must verifiy if variable isLoggedIn is used
  bool isLoggedIn;
  sf::RenderWindow mainWindow;
  tgui::Gui AuthGui;
  void initWidget();
  void updateTextSize();
  void processEvents();
  void login(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  bool signIn(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  void menuWindow();
  bool checkLogin();
  void loginWidget();
  // void menuWidget();

public:
  AuthWindow();
  void run();
};
#endif
