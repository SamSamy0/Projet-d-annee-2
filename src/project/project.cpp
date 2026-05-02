#include "project.hpp"
#include "map.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <memory>
#include <string>

const string FONT_PATH{"../res/police/ARIAL.TTF"};

// Constructors
Project::Project(unsigned int scale, sf::Vector2u size, std::string name,
                 uint id, sf::RenderWindow &window, tgui::Gui &gui,
                 ClientNetworkManager &manager, int8_t role)
    : map_{std::make_shared<Map>(id_, size, scale)}, name_{name}, id_{id},
      window_{window}, toolbar_{map_, manager}, gui_{gui}, role_{role} {}

Project::Project(unsigned int scale, sf::Vector2u size, std::string name,
                 uint id, sf::RenderWindow &window, tgui::Gui &gui,
                 ClientNetworkManager &manager,
                 std::vector<std::shared_ptr<Layer>> layers, int8_t role)
    : map_{std::make_shared<Map>(id_, size, scale, layers)}, name_{name},
      id_{id}, window_{window}, toolbar_{map_, manager}, gui_{gui},
      role_{role} {}

// ----- [Getters] -----
uint Project::getId() { return id_; }
unsigned int Project::getScale() { return map_->getScale(); }

std::string Project::getName() { return name_; }

ToolBar &Project::getToolBar() { return toolbar_; }

Chat &Project::getChat() { return chat_; }

void Project::setChat(const Chat chat) { chat_ = chat; }

std::shared_ptr<Map> Project::getMap() { return map_; }

sf::View &Project::getView() { return viewMap_; }

// Setters
void Project::setName(std::string name) { name_ = name; }

// Display
void Project::displayScale() {
  sf::Font police(FONT_PATH);
  police.setSmooth(true);
  sf::Text scaleText(police);

  scaleText.setString("1 m = " + to_string(getScale()) + " px");
  scaleText.setCharacterSize(static_cast<unsigned int>(window_.getSize().y * 0.02f));
  scaleText.setFillColor(sf::Color::White);
  scaleText.setPosition(sf::Vector2f(
      window_.getSize().x - scaleText.getLocalBounds().size.x - 10.f,
      window_.getSize().y * 0.01f));

  window_.setView(window_.getDefaultView());
  window_.draw(scaleText);
}

void Project::display() {
  window_.clear(sf::Color(36, 40, 47));
  // ------ [ afficher la carte ] -----
  viewMap_.setViewport(sf::FloatRect{{0.21f, 0.05f}, {0.79f, 0.95f}});
  window_.setView(viewMap_);
  map_->displayMap(window_, viewMap_);
  window_.setView(window_.getDefaultView());

  displayScale();
}

void Project::setId(uint newId) {
  id_ = newId;
  map_->setId(newId);
}

int8_t Project::getRole() { return role_; }
void Project::setRole(int8_t newRole) { role_ = newRole; }
