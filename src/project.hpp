#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "map.hpp"


class Tool;



class Project{
  // Chat chat;
  std::unique_ptr<Map> map_;
  std::vector<Tool> tools_;


public:
  float getScale(); 

};

