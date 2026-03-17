#pragma once

#include "../Window/Window.hpp"
#include <cstdint>
#include <string>
#include <vector>

class Window; // Forward declaration

class ReceiverInWindow {
  Window *window_;
  

public:
  ReceiverInWindow(Window &w);
  //Method on Window
  void switchConnectState(uint8_t connect);
  void addProjectToList(ProjectData projet);
  void updateProjectNameInList(long long id, const std::string &name);
  void updateCreatedProjectId(uint32_t ProjectId);
  void setState();
  
  //Method on Project
  
};
