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
  void drawSquare(uint64_t layer_id, int pos_x,int pos_y, float size, uint8_t r, uint8_t g,uint8_t b, uint8_t a);
  void drawCircle(uint64_t layer_id,int pos_x,int pos_y,float size, uint8_t r, uint8_t g,uint8_t b, uint8_t a);
  void drawDiamond(uint64_t layer_id,int pos_x,int pos_y,float size_x, uint8_t r, uint8_t g,uint8_t b, uint8_t a);
  void eraseSquare(uint64_t layer_id,int pos_x,int pos_y,float size);
  void eraseCircle(uint64_t layer_id,int pos_x,int pos_y,float size);
  void eraseDiamond(uint64_t layer_id,int pos_x,int pos_y,float size_x,float size_y);
    

  void setState();

  // Method on Project
  void drawOnProject();
};
