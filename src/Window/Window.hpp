#pragma once

#include "../client/clientnetwork.hpp"
#include "../project/Layer/layer.hpp"
#include "../project/map.hpp"
#include "../project/project.hpp"
#include "../project/user.hpp"
#include "ProjectData.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <memory>
#include <vector>

enum class projectState { LOGIN, MENU, GAME };
enum class focusPopup { CREATE, RENAME, DUPLICATE };

class Window {
private:
  ClientNetworkManager &manager;
  sf::RenderWindow mainWindow;

  bool isWatingRep = false;
  bool isToolSelected_ = false;
  User currentUser_;

  projectState state = projectState::LOGIN;
  bool isLoggedIn = false;
  // Authentification Interface
  tgui::Gui gui;
  std::unique_ptr<Project> project;

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

  // Window -> Application
  void initWidget();
  void processEvents();
  void handleWindowEvents(const std::optional<sf::Event> &event);
  void handlePopupEvents(const std::optional<sf::Event> &event);
  void updateTextSize();

  // Login Window -> LoginView
  void login(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  void signIn(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  void loginWidget();
  bool checkLogin();

  // Menu Window
  void initMenuWidget();
  void shareProj();
  void joinProj(tgui::Panel::Ptr panel);
  void displayProjList(tgui::Panel::Ptr parent);
  void showProjectMenu(ProjectData, tgui::Button::Ptr toHover);
  void initInputWidget(focusPopup focus, ProjectData project = ProjectData{});
  void popupRename(tgui::Panel::Ptr back, ProjectData project, focusPopup view);
  void popupCreate(tgui::Panel::Ptr background);
  void popupDuplicate(tgui::Panel::Ptr background, ProjectData project,
                      focusPopup view);
  void exitAction(tgui::Panel::Ptr background);
  void closePopup();
  void createPopup();
  bool checkInput(tgui::EditBox::Ptr scale, tgui::EditBox::Ptr sizeX,
                  tgui::EditBox::Ptr sizeY);
  bool boxError(tgui::EditBox::Ptr box);
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
  void setState(projectState newState);
  // Constructor
  Window(ClientNetworkManager &manager);
  Window();
  // Setter for bool isLoggedIn
  void setLogIn();
  // Setter for projectList
  void addProjectList(ProjectData projet);
  void updateProjectNameInList(long long id, const std::string &name);
  void updateList();
  bool isOpen() const;
  void run();
  void updateCreatedProjectId(uint32_t projId);
};
