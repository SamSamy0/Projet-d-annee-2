#pragma once

#include "../Window/Application.hpp"
#include "../server/datamanager/memberentry.hpp"
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
  void setUserId(uint newId);
  void addProjectToList(ProjectData projet);
  void updateProjectNameInList(uint id, const std::string &name);
  void updateCreatedProjectId(uint ProjectId);
  void createLayer(LayerType type);
  void deleteLayer(uint current_layer_id);
  void renameLayer(uint layer_id, std::string name);
  void layerUp(uint layer_id);
  void layerDown(uint layer_id);
  void drawPixelBrush(uint layer_id, int pos_x, int pos_y, uint8_t r, uint8_t g,
                      uint8_t b, uint8_t a, Shape shape, bool eraser,
                      float size_x, float size_y);
  void drawSprite(uint layer_id, std::string asset_id, int pos_x, int pos_y, float size);
  void eraseSprite(uint layer_id,uint sprite_id);
  void shiftLayer(uint layer_id, int delta_x, int delta_y);
  void updateShareToken(std::string newTok);
  void clearProjList();
  void clearMemberList();
  void setMemberList(std::vector<MemberEntry> memberList);
  void updateMemberList(uint projectId, uint target, int8_t role);
  void setState();
  void addProjectData(unsigned int scale, sf::Vector2u size, std::string name,
                      uint id);
  void addProjectData(unsigned int scale, sf::Vector2u size, std::string name,
                      uint id, uint nextLayerId,
                      const std::vector<LayerLoadData>& layers);
};
