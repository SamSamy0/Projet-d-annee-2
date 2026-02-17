#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "project.hpp"
#include "map.hpp"
#include "layer.hpp"
#include "tool.hpp"
#include <memory>
#include "toolbar.hpp"



Project::Project(unsigned int scale, sf::Vector2u size, sf::RenderWindow &window, tgui::Gui& gui): map_{std::make_shared<Map>(1,size,scale)},
  window_{window},toolbar_{map_}, gui_{gui} {}

unsigned int Project::getScale() { return map_->getScale(); }
std::shared_ptr<Map> Project::getMap(){return map_;}
ToolBar Project::getToolBar(){return toolbar_;}

void Project::displayScale() {
  sf::Font police("police/ARIAL.TTF");
  sf::Text scaleText(police);
  scaleText.setString("1 metre =" + dynamic_cast<strinf>(getScale()) + " pixels");
  scaleText.setCharacterSize(17);
  scaleText.setFillColor(sf::Color::White);
  scaleText.setPosition(sf::Vector2f(850, 13));
  window_.draw(scaleText);
}

void Project::display() {
  window_.clear(sf::Color(200,200,200));
  // ------ [ afficher la carte ] -----
  window_.setView(viewMap_);
  map_->displayMap(window_, viewMap_);
  
  sf::RectangleShape menuBar;
  menuBar.setSize(sf::Vector2f(1200, 50));
  menuBar.setFillColor(sf::Color(26, 188, 187));

  sf::RectangleShape menuBarLeft;
  menuBarLeft.setSize(sf::Vector2f(50, 1200));
  menuBarLeft.setFillColor(sf::Color(200, 200, 200));

  window_.setView(viewUI_);
  
  window_.draw(menuBarLeft);

  sf::VertexArray topBar(sf::PrimitiveType::Triangles, 6);

  float width = window_.getSize().x;
  float height = 50.f;

  // Je paramètre la menu au dessus, j'utilise des triangles pour le faire car plus de formes à 4 cotés pour des dégradés depuis SFML 3
  topBar[0].position = {0, 0};
  topBar[1].position = {width, 0};
  topBar[2].position = {width, height};
  topBar[3].position = {0, 0};
  topBar[4].position = {width, height};
  topBar[5].position = {0, height};

  // Je met des couleurs au différents points pour faire un dégradé
  topBar[0].color = sf::Color(26,188,187);
  topBar[1].color = sf::Color(70,120,255);
  topBar[2].color = sf::Color(70,120,255);
  topBar[3].color = sf::Color(26,188,187);
  topBar[4].color = sf::Color(70,120,255);
  topBar[5].color = sf::Color(26,188,187);

  window_.draw(topBar);
  displayScale();
  gui_.draw();
  window_.display();
}

