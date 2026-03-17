#pragma once

#include <cstdint>
#include <string>
#include <vector>
// #include "../Window/Window.hpp"
#include "../Window/Application.hpp"

class Window; // Forward declaration

// class Window;             // Forward declaration

class ReceiverInWindow {
  Application *app_;

public:
  ReceiverInWindow(Application &w);
  void switchConnectState(uint8_t connect);
  void addProjectToList(ProjectData projet);
  void updateProjectNameInList(uint64_t id, const std::string &name);
  void updateCreatedProjectId(uint32_t ProjectId);
  void setState();
  void addProjectData(unsigned int scale, sf::Vector2u size, std::string name, uint id);
};
