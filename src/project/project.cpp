#include "project.hpp"
#include "map.hpp"
#include "layer.hpp"
#include "tool.hpp"
#include <memory>
#include "toolbar.hpp"



Project::Project(unsigned int scale, sf::Vector2u size, sf::RenderWindow &window): map_{std::make_shared<Map>(1,size,scale)},
  window_{window},toolbar_{map_}{}

unsigned int Project::getScale() { return map_->getScale(); }
std::shared_ptr<Map> Project::getMap(){return map_;}
ToolBar Project::getToolBar(){return toolbar_;}

void Project::displayScale() {
  sf::Font police("police/ARIAL.TTF");
  sf::Text scaleText(police);
  scaleText.setString("1 metre = 5 pixel");
  scaleText.setCharacterSize(17);
  scaleText.setFillColor(sf::Color::White);
  scaleText.setPosition(sf::Vector2f(850, 13));
  window_.draw(scaleText);
}

void Project::displayToolBar() {
  sf::Texture homeTexture;
  homeTexture.loadFromFile("images/home.png");
  sf::Sprite homeSprite{homeTexture};
  //homeSprite.setPosition()
}

void Project::display() {
  window_.clear(sf::Color(80,80,80));
  // ------ [ afficher la carte ] -----
  window_.setView(viewMap_);
  map_->displayMap(window_, viewMap_);
  
  sf::RectangleShape menuBar;
  menuBar.setSize(sf::Vector2f(1200, 50));
  menuBar.setFillColor(sf::Color(50,50,50));

  window_.setView(viewUI_);
  window_.draw(menuBar);
  displayScale();
  displayToolBar();

  window_.display();
}

