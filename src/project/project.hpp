#pragma once
#include "toolbar.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "Chat/chat.hpp"


class Tool;
class Map;
class User;

class Project{
    // std::vector<User> users;
    Chat chat_;
    std::string name_;
    uint id_;
    sf::RenderWindow& window_;
    tgui::Gui& gui_;
    sf::View viewMap_;
    sf::View viewUI_;
    std::shared_ptr<Map> map_;
    ToolBar toolbar_;

public:
    Project(unsigned int scale, sf::Vector2u size, std::string name, uint id, sf::RenderWindow& window, tgui::Gui& gui);
    Project(unsigned int scale, sf::Vector2u size, std::string name, uint id, sf::RenderWindow& window, tgui::Gui& gui, std::vector<std::shared_ptr<Layer>> layers);
    uint getId();
    std::string getName();
    void setName(std::string name);
    unsigned int getScale();
    std::shared_ptr<Map> getMap();
    ToolBar& getToolBar();
    Chat& getChat();
    void display();
    void displayScale();
    void displayToolBar();
    void displayBackground();
    sf::View& getView();
};
