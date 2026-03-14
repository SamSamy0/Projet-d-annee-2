#pragma once

#include "../project/project.hpp"
#include "../project/user.hpp"
#include "../client/clientnetwork.hpp"
#include "../project/map.hpp"
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
  sf::RenderWindow mainWindow;

  bool isWatingRep = false;
  bool isToolSelected_ = false;
  User currentUser_;

  // WARNING: Changer MENU à LOGIN pour la vrai version, là c'est par facilité
  projectState state = projectState::LOGIN;
  bool isLoggedIn = false;
  // Authentification Interface
  tgui::Gui gui;
  std::unique_ptr<Project> project;
  // Temporary (until server is OK)
  std::vector<ProjectData> projectList = {};

  tgui::Panel::Ptr layerPanel_ = nullptr;
  tgui::ScrollablePanel::Ptr layersList_ = nullptr;
  tgui::Button::Ptr activeMoreButton = nullptr;

  // Chat widget
  tgui::Panel::Ptr chatPanel_ = nullptr;
  tgui::ScrollablePanel::Ptr chatMessages_ = nullptr;
  tgui::EditBox::Ptr chatInput_ = nullptr;
  tgui::Button::Ptr chatSendButton_ = nullptr;

  // Pen options panel
  tgui::Panel::Ptr penOptionsPanel_ = nullptr;
  // Eraser options panel
  tgui::Panel::Ptr eraserOptionsPanel_ = nullptr;

  // Window
  void initWidget();
  void processEvents();
  void handleWindowEvents(const std::optional<sf::Event> &event);
  void handlePopupEvents(const std::optional<sf::Event> &event);
  void setState(projectState newState);
  void updateTextSize();

  // Login Window
  void login(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  void signIn(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  void loginWidget();
  bool checkLogin();

  // Menu Window
  void initMenuWidget();
  void displayProjList(tgui::Panel::Ptr parent);
  void showProjectMenu(long long id, tgui::Button::Ptr toHover);
  void closePopup();
  void initDataWidget();
  void createProj(tgui::String scale, tgui::String sizeX, tgui::String sizeY,
                  tgui::String name, unsigned int id, sf::RenderWindow &window,
                  tgui::Gui &gui);
  ProjectData getProjectData(std::unique_ptr<Project> &proj);
  ProjectData askProjectData();

  // Game Window
  void initGameWidget();
  void initToolbar();
  void initLayerPanel();
  void initChatWidget();
  void initPenOptions();
  void initEraserOptions();
  void refreshChat();
  void refreshLayerList();
  void handleGameEvents(const std::optional<sf::Event> &event);
  // Detection in map
  void toolOnClick();
  void toolOnRelease();
  void checkTypeTool(LayerType previous_type);

public:
  // Constructor
  Window(ClientNetworkManager &manager);
  Window();
  // Setter for bool isLoggedIn
  void setLogIn();
  // Setter for projectList
  void addProjectList(ProjectData projet);
  bool isOpen() const;
  void run();
  
};
