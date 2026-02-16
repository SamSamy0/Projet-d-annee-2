#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
<<<<<<< HEAD
#include "map.hpp"
=======
#include "toolbar.hpp"
>>>>>>> a2ad2e28b449ff6dc02f111fa1fb80e2395bd9e9

class Tool;
class Map;
class User;

class Project{
  // std::vector<User> users;
  // Chat chat;
  sf::RenderWindow& window_;
  sf::View viewMap_;
  sf::View viewUI_;
  std::shared_ptr<Map> map_;
  ToolBar toolbar_;

public:
  Project(unsigned int scale, sf::Vector2u size, sf::RenderWindow& window);
  unsigned int getScale();
  std::shared_ptr<Map> getMap();
<<<<<<< HEAD
  std::vector<std::shared_ptr<Tool>> getTools();
  void display();
  void displayScale();
=======
  ToolBar getToolBar();
>>>>>>> a2ad2e28b449ff6dc02f111fa1fb80e2395bd9e9
};
