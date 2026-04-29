#include "project.hpp"
#include "Layer/layer.hpp"
#include "map.hpp"
#include "miniz.h"
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

void Project::exportToPng() {
  // Size of the wanted zone
  sf::Vector2f viewSize = viewMap_.getSize();

  sf::RenderTexture exportTexture({static_cast<unsigned int>(viewSize.x),
                                   static_cast<unsigned int>(viewSize.y)});

  // Setting export texture like its recording viewMap(the blue square)
  exportTexture.setView(viewMap_);
  exportTexture.clear(sf::Color::Transparent);

  // Transforming the whole map into Sprite in order to draw it on the png
  sf::Sprite mapSprite(map_->getRenderTexture().getTexture());

  // Drawing the big map into the png
  // Thanks to setView(), we only draw the blue square
  exportTexture.draw(mapSprite);

  // Actually drawing on the png
  exportTexture.display();

  sf::Image finalImage = exportTexture.getTexture().copyToImage();
  std::string filepath = "../png/" + name_ + ".png";

  if (finalImage.saveToFile(filepath)) {
    std::cout << "Succès: zone de base exporté vers " << filepath << std::endl;
  } else {
    std::cerr << "Erreur lors de l'exportation vers " << filepath << std::endl;
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
void Project::exportToNative() {
  // Path
  std::string zipPath = "../nativExport/" + name_ + ".zip";
  std::string originalImagesPath =
      "../bin/projectsFolder/project_" + to_string(id_) + "/images/";
  std::string destImagePath = "project_" + to_string(id_) + "/images/";

  // Creating zip file
  mz_zip_archive zip_archive;
  mz_zip_zero_struct(&zip_archive);
  // Filling zip with 0
  mz_zip_writer_init_file(&zip_archive, zipPath.c_str(), 0);

  // Writing in zipFile
  // BUG: Quand un projet est créer et on essaye d'exporter, ça ne fonctionne
  // pas car on ne sauvegarde en local qu'au moment où on ferme l'appli
  for (const auto &entry :
       std::filesystem::recursive_directory_iterator(originalImagesPath)) {
    std::string fullPathOnDisk = entry.path().string();

    std::string relativePath =
        std::filesystem::relative(entry.path(), originalImagesPath).string();
    std::string FileInZipPath = destImagePath + relativePath;

    mz_zip_writer_add_file(&zip_archive, FileInZipPath.c_str(),
                           fullPathOnDisk.c_str(), NULL, 0,
                           MZ_BEST_COMPRESSION);
  }

  // Finalisation et nettoyage
  mz_zip_writer_finalize_archive(&zip_archive);
  mz_zip_writer_end(&zip_archive);

  std::cout << "fin de la compression " << std::endl;
}
