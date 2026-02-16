#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "toolbar.hpp"

class Tool;
class Map;
class User;

class Project{
  // std::vector<User> users;
  // Chat chat;
  std::shared_ptr<Map> map_;
  ToolBar toolbar_;


public:
  Project(unsigned int scale, sf::Vector2u size);
  unsigned int getScale();
  std::shared_ptr<Map> getMap();
  ToolBar getToolBar();
};
