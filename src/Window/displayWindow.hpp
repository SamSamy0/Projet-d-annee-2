#pragma once
#include "../project/project.hpp"
#include "../server/clientnetwork.hpp"
#include "ProjectData.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <memory>
#include <vector>

enum class projectState { LOGIN, MENU, GAME };
class Window {
private:
  ClientNetworkManager &manager;
  // WARNING: Changer MENU à LOGIN pour la vrai version, là c'est par facilité
  projectState state = projectState::LOGIN;
  bool isLoggedIn = false;
  sf::RenderWindow mainWindow;
  // Authentification Interface
  tgui::Gui gui;
  std::unique_ptr<Project> project;
  // Temporary (until server is OK)
  std::vector<ProjectData> projectList = {
      {1001LL, 1, "Alpha System"}, {1002LL, 2, "Beta Scanner"},
      {1003LL, 3, "Gamma Driver"}, {1004LL, 1, "Delta Interface"},
      {1005LL, 2, "Epsilon API"},  {1006LL, 1, "Zeta Database"},
      {1007LL, 3, "Eta Renderer"}, {1008LL, 2, "Theta Parser"},
      {1009LL, 1, "Iota Module"},  {1010LL, 3, "Kappa Cloud"},
      {1011LL, 2, "Lambda Logic"}, {1012LL, 1, "Mu Mobile"},
      {1013LL, 1, "Nu Network"},   {1014LL, 2, "Xi Compiler"},
      {1015LL, 3, "Omicron OS"},   {1016LL, 1, "Pi Physics"},
      {1017LL, 2, "Rho Router"},   {1018LL, 1, "Sigma Shader"},
      {1001LL, 1, "Alpha System"}, {1002LL, 2, "Beta Scanner"},
      {1003LL, 3, "Gamma Driver"}, {1004LL, 1, "Delta Interface"},
      {1005LL, 2, "Epsilon API"},  {1006LL, 1, "Zeta Database"},
      {1007LL, 3, "Eta Renderer"}, {1008LL, 2, "Theta Parser"},
      {1009LL, 1, "Iota Module"},  {1010LL, 3, "Kappa Cloud"},
      {1011LL, 2, "Lambda Logic"}, {1012LL, 1, "Mu Mobile"},
      {1013LL, 1, "Nu Network"},   {1014LL, 2, "Xi Compiler"},
      {1015LL, 3, "Omicron OS"},   {1016LL, 1, "Pi Physics"},
      {1017LL, 2, "Rho Router"},
  };

  tgui::ScrollablePanel::Ptr layersList_ = nullptr;
  void initWidget();
  void updateTextSize();
  void processEvents();
  void login(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  void signIn(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  void initMenuWidget();
  bool checkLogin();
  void loginWidget();
  // void createProj();
  void createProj(tgui::String scale, tgui::String sizeX, tgui::String sizeY,
                  tgui::String name, unsigned int id, sf::RenderWindow &window,
                  tgui::Gui &gui);
  ProjectData getProjectData(std::unique_ptr<Project> &proj);
  void setState(projectState newState);
  void displayProjList(tgui::Panel::Ptr parent);
  void showProjectMenu(long long id);
  void initDataWidget();
  ProjectData askProjectData();
  void refreshLayerList();
  // Detection in map
  void toolOnClick();
  void toolOnRelease();

public:
  // Constructor
  Window(ClientNetworkManager &manager);
  Window();
  // Setter for bool isLoggedIn
  void setLogIn();
  // Setter for projectList
  void setProjectList(std::vector<ProjectData> list);
  void run();
};
