#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>


class Tool;
class Map;



class Project{
  // Chat chat;
  std::unique_ptr<Map> map_;
  std::vector<Tool> tools_;


public:
  Project(); //TODO: à définir avec Adam
  unsigned int getScale(); 

};

