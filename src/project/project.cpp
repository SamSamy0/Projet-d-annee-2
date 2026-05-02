#include "project.hpp"
#include "Layer/layer.hpp"
#include "map.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <filesystem>
#include <memory>
#include <string>

const string FONT_PATH{"../res/police/ARIAL.TTF"};

using recursive_directory_iterator =
    std::filesystem::recursive_directory_iterator;

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
  scaleText.setCharacterSize(17);
  scaleText.setFillColor(sf::Color::White);
  scaleText.setPosition(sf::Vector2f(
      window_.getSize().x - scaleText.getLocalBounds().size.x - 10.f, 10.f));

  window_.setView(window_.getDefaultView());
  window_.draw(scaleText);
}

bool Project::exportToPng(const std::string &extension) {
  const sf::Texture &mapTexture = map_->getRenderTexture().getTexture();
  sf::Image finalImage = mapTexture.copyToImage();
  std::string filepath = "../export_image/" + name_ + "." + extension;

  if (finalImage.saveToFile(filepath)) {
    std::cout << "Succès: carte complète exportée vers " << filepath
              << std::endl;
    return true;
  } else {
    std::cerr << "Erreur lors de l'exportation vers " << filepath << std::endl;
    return false;
  }
}
void Project::displayBackground() {
  sf::RectangleShape top;

  top.setSize(sf::Vector2f(window_.getSize().x, window_.getSize().y * 0.07));
  top.setFillColor(sf::Color(36, 40, 47));

  sf::RectangleShape left;

  left.setPosition(sf::Vector2f(0, window_.getSize().y * 0.05));
  left.setSize(
      sf::Vector2f(window_.getSize().x * 0.18, window_.getSize().y * 1.5));
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

void Project::setId(uint newId) {
  id_ = newId;
  map_->setId(newId);
}

int8_t Project::getRole() { return role_; }
void Project::setRole(int8_t newRole) { role_ = newRole; }
