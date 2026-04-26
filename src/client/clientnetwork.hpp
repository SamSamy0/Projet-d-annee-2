#pragma once
#include "../common/protocol.hpp"
#include "../project/Layer/layer.hpp"
#include "serverevent.hpp"
#include <SFML/Network.hpp>
#include <deque>
#include <memory>

class ClientEventHandler;

class ClientNetworkManager {
  sf::TcpSocket socket_;
  std::deque<ServerEvent> reponse_;

public:
  bool connect();
  void getEvent();
  bool hasEvent();
  ServerEvent popEvent();
  std::deque<ServerEvent> &getQueuEvent();
  void login(std::string pseudo, std::string password);
  void askRegister(std::string pseudo, std::string password);
  void createProject(std::string role, sf::Vector2u size, uint scale);
  void renameProject(uint projectId, std::string newName);
  void dupProj(uint projectId, std::string newName);
  void getUsersProjects(uint id);
  void getProjectList();
  void createProjectCode(uint8_t role, uint project_id);
  void delProject(uint project_id);
  void getProjectData(uint project_id);
  void joinProject(std::string project_code);
  void kickUser(uint targetId, uint projectId);
  void createLayer(uint proj_id, LayerType type);
  void deleteLayer(uint proj_id, uint current_layer_id);
  void renameLayer(uint proj_id, uint layer_id, std::string name);
  void layerUp(uint proj_id, uint current_layer_id);
  void layerDown(uint proj_id, uint current_layer_id);
  void leaveProject(uint project_id);
  void changeRole(uint userId, uint projectId, int8_t role);

  void drawSquare(uint proj_id, uint layer_id, int pos_x, int pos_y, float size, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
  void drawCircle(uint proj_id, uint layer_id, int pos_x, int pos_y, float size, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
  void drawDiamond(uint proj_id, uint layer_id, int pos_x, int pos_y, float size_x, float size_y, uint8_t r, uint8_t g, uint8_t b,uint8_t a);
  void eraseSquare(uint proj_id, uint layer_id, int pos_x, int pos_y, float size);
  void eraseCircle(uint proj_id, uint layer_id, int pos_x, int pos_y, float size);
  void eraseDiamond(uint proj_id, uint layer_id, int pos_x, int pos_y, float size_x, float size_y);
  void drawSprite(uint proj_id, uint layer_id, std::string asset_id, int pos_x, int pos_y, float size_x);
  void eraseSprite(uint proj_id, uint layer_id, uint sprite_id);

  void moveSprites(uint proj_id, uint layer_id, const std::vector<uint>& sprite_ids, sf::Vector2i v);
  void resizeSprites(uint proj_id, uint layer_id, const std::vector<uint>& sprite_ids, const std::vector<sf::Vector2f>& positions, float scale);
  void rotateSprites(uint proj_id, uint layer_id, const std::vector<uint>& sprite_ids, float angle, const std::vector<sf::Vector2f>& positions);

  void shiftLayer(uint proj_id, uint layer_id, int delta_x, int delta_y);

  void sendMessageChat(std::string message);
};
