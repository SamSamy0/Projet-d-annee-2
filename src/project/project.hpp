#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "map.hpp"
#include "toolbar.hpp"

class Tool;
class Map;
class User;

class Project{
  // std::vector<User> users;
  // Chat chat;
  std::shared_ptr<Map> map_;
  sf::RenderWindow& window_;
  sf::View viewMap_;
  sf::View viewUI_;
  ToolBar toolbar_;

public:
  Project(unsigned int scale, sf::Vector2u size, sf::RenderWindow& window);
  unsigned int getScale();
  std::shared_ptr<Map> getMap();
  void display();
  void displayScale();
  ToolBar getToolBar();
  void displayToolBar();
};
