#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <memory>
#include <string>
#include "project.hpp"
#include "map.hpp"

const string FONT_PATH {"../res/police/ARIAL.TTF"};

// Constructors
Project::Project(unsigned int scale, sf::Vector2u size, std::string name, uint id, sf::RenderWindow &window, tgui::Gui &gui)
    : map_{std::make_shared<Map>( size, scale)}, name_{name}, id_{id},window_{window}, toolbar_{map_}, gui_{gui} {}

Project::Project(unsigned int scale, sf::Vector2u size, std::string name, uint id, sf::RenderWindow &window, tgui::Gui &gui, std::vector<std::shared_ptr<Layer>> layers)
    : map_{std::make_shared<Map>( size, scale,layers)}, name_{name}, id_{id},window_{window}, toolbar_{map_}, gui_{gui} {}

// ----- [Getters] -----
uint Project::getId() { return id_; }
unsigned int Project::getScale() { return map_->getScale(); }

std::string Project::getName() { return name_; }

ToolBar& Project::getToolBar() { return toolbar_; }

Chat& Project::getChat() { return chat_; }

std::shared_ptr<Map> Project::getMap() { return map_; }

sf::View &Project::getView() { return viewMap_; }

// Setters
void Project::setName(std::string name) { name_ = name; }


// Display
void Project::displayScale() {
  sf::Font police(FONT_PATH);
  sf::Text scaleText(police);

  scaleText.setString("1m = " + to_string(getScale()) + " px");
  scaleText.setCharacterSize(17);
  scaleText.setFillColor(sf::Color::Black);
  scaleText.setPosition(sf::Vector2f(window_.getSize().x * 0.16f, window_.getSize().y * 0.97f));

  window_.draw(scaleText);
}

void Project::displayBackground() {
  sf::RectangleShape top;

  top.setSize(sf::Vector2f(window_.getSize().x, window_.getSize().y * 0.07));
  top.setFillColor(sf::Color(36, 40, 47));

  sf::RectangleShape left;

  left.setPosition(sf::Vector2f(0, window_.getSize().y * 0.05));
  left.setSize(sf::Vector2f(window_.getSize().x * 0.15, window_.getSize().y));
  left.setFillColor(sf::Color(36, 40, 47));

  window_.draw(top);
  window_.draw(left);
}

void Project::display() {
  window_.clear(sf::Color(200, 200, 200));
  // ------ [ afficher la carte ] -----
  window_.setView(viewMap_);
  map_->displayMap(window_, viewMap_);
  window_.setView(viewUI_);

  displayBackground();
  displayScale();
}
