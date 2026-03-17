#pragma once

#include "../Window/Application.hpp"
#include "../project/Tool/brush.hpp"
#include "../project/Tool/pixelbrush.cpp"
#include <cstdint>
#include <string>
#include <vector>

class Window; // Forward declaration

class ReceiverInWindow {
  Application *app_;
  sf::Color color_;
  Shape shape_;

public:
  ReceiverInWindow(Application &w);
  // Method on Window
  void switchConnectState(uint8_t connect);
  void addProjectToList(ProjectData projet);
  void updateProjectNameInList(long long id, const std::string &name);
  void updateCreatedProjectId(uint32_t ProjectId);
  void setState();

  // Method on Project
  void drawOnProject();
};
