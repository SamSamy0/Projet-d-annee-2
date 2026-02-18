#ifndef DISPLAYWINDOW_HPP
#define DISPLAYWINDOW_HPP
#include "../server/clientnetwork.hpp"
#include "Identifier.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <vector>
#include "../project/project.hpp"

class Project;
enum class projectState { LOGIN, MENU, GAME };
class Window {
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
  std::unique_ptr<Project> project = nullptr;
  tgui::Gui gui;
  std::unique_ptr<Project> carteRPG_;
  void initWidget();
  void updateTextSize();
  void processEvents();
  void login(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  void signIn(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  void initMenuWidget();
  bool checkLogin();
  void loginWidget();
  void createProj();
  // void menuWidget();

  // Detection
  void leftClickEvent();

public:
  // Constructor
  Window(ClientNetworkManager &manager);
  // Setter for bool isLoggedIn
  void setLogIn();
  void run();
};
#endif
