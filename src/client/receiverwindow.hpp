#pragma once

#include "../Window/Application.hpp"
#include <cstdint>
#include <string>
#include <vector>

class Window; // Forward declaration

class ReceiverInWindow {
  Application *app_;

public:
  ReceiverInWindow(Application &w);
  // Method on Window
  void switchConnectState(uint8_t connect);
  void addProjectToList(ProjectData projet);
  void updateProjectNameInList(uint id, const std::string &name);
  void updateCreatedProjectId(uint ProjectId);
  void drawPixelBrush(uint layer_id, int pos_x, int pos_y, uint8_t r, uint8_t g,
                      uint8_t b, uint8_t a, Shape shape, bool eraser,
                      float size_x, float size_y);
  void shiftLayer(uint layer_id, int delta_x, int delta_y);

  void setState();
  void addProjectData(unsigned int scale, sf::Vector2u size, std::string name,
                      uint id);
};
