#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "map.hpp"

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
  std::vector<std::shared_ptr<Tool>> tools_;

public:
  Project(unsigned int scale, sf::Vector2u size, sf::RenderWindow& window);
  unsigned int getScale();
  std::shared_ptr<Map> getMap();
  std::vector<std::shared_ptr<Tool>> getTools();
  void display();
  void displayScale();
};
