#ifndef DISPLAYWINDOW_HPP

#define DISPLAYWINDOW_HPP
#include "Identifier.hpp"
#include "server/clientnetwork.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <vector>

enum class projectState { LOGIN, MENU, GAME };
class AuthWindow {
private:
  ClientNetworkManager &manager;
  projectState state = projectState::LOGIN;
  // Temporary
  std::vector<Identifier> DB;
  // Must verifiy if variable isLoggedIn is used
  bool isLoggedIn;
  sf::RenderWindow mainWindow;
  // sf::RenderWindow menuWindow;
  // Authentification Interface
  tgui::Gui gui;
  void initWidget();
  void updateTextSize();
  void processEvents();
  void login(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  void signIn(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  void initMenuWidget();
  bool checkLogin();
  void loginWidget();
  // void menuWidget();

public:
  // Constructor
  AuthWindow(ClientNetworkManager &manager);
  // Setter for bool isLoggedIn
  void setLogIn();
  void run();
};
#endif
