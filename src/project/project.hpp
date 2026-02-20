#pragma once
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
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
    std::string name_;
    unsigned int id_;
    sf::RenderWindow& window_;
    tgui::Gui& gui_;
    sf::View viewMap_;
    sf::View viewUI_;
    std::shared_ptr<Map> map_;
    ToolBar toolbar_;

public:
    Project(unsigned int scale, sf::Vector2u size, std::string name, unsigned int id, sf::RenderWindow& window, tgui::Gui& gui);
    unsigned int getId();
    std::string getName();
    void setName(std::string name);
    unsigned int getScale();
    std::shared_ptr<Map> getMap();
    ToolBar& getToolBar();
    void display();
    void displayScale();
    void displayToolBar();
    void displayLeftBar();
    void displayBackground();
    sf::View& getView();
};
