#pragma once
#include "../project/project.hpp"
#include "../server/clientnetwork.hpp"
#include "Identifier.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <vector>

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
  // Authentification Interface
  tgui::Gui gui;
  std::unique_ptr<Project> project = nullptr;
  void initWidget();
  void updateTextSize();
  void processEvents();
  void login(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  void signIn(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  void initMenuWidget();
  bool checkLogin();
  void loginWidget();
  void createProj();
  void setState(projectState newState);
  // void menuWidget();

  // Detection in map
  void toolOnClick();
  void toolOnRelease();

public:
  // Constructor
  Window(ClientNetworkManager &manager);
  Window();
  // Setter for bool isLoggedIn
  void setLogIn();
  void run();
};
