#pragma once
#include "../client/clientnetwork.hpp"
#include "../project/Layer/layer.hpp"
#include "../project/map.hpp"
#include "../project/project.hpp"
#include "../project/user.hpp"
#include "ProjectData.hpp"
#include "View.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <memory>
#include <vector>

class Application {
  ClientNetworkManager &manager;
  sf::RenderWindow mainWindow;
  tgui::Gui gui;
  projectState state = projectState::LOGIN;
  bool isLoggedIn = false;
  std::unique_ptr<Project> project;
  User currentUser_;
  std::unique_ptr<View> currentView;
  int8_t currentProjRole;

public:
  Application(ClientNetworkManager &manager);
  Application();
  // Method for ReceiverInWindow
  void addProjectList(ProjectData project);
  void clearProjList();
  void updateProjectNameInList(long long id, const std::string &newName);
  void updateCreatedProjectId(uint32_t projId);
  void updateShareToken(std::string newToken);
  void setCurrentProjRole(int8_t role) { currentProjRole = role; };
  void setUserId(uint userId) { currentUser_.setId(userId); }

  void changeView(std::unique_ptr<View> newView);
  void showLoginError(const std::string &message);
  void setState(projectState newState);
  void processEvents();
  void run();
  bool isOpen() const;
  void updateTextSize();
  void loadProjectData(unsigned int scale, sf::Vector2u size, std::string name,
                       uint id);
  // Getters
  sf::RenderWindow &getWindow();
  tgui::Gui &getGui();
  ClientNetworkManager &getNetwork();
  User &getUser();
  std::unique_ptr<View> &getCurrentView();
  std::unique_ptr<Project> &getProject();
};
