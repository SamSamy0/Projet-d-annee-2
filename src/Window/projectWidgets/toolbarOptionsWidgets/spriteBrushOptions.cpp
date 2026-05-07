#include "../../Application.hpp"
#include "../GameView.hpp"
#include "../../../project/Tool/spritebrush.hpp"
#include <portable-file-dialogs.h>

void GameView::initSpriteBrushOptions() {
  auto& gui = app_.getGui();

  // Ici, nous avons le panel principale, c'est ici où tout les images, tabs vont s'afficher
  spriteBrushOptionsPanel_ = tgui::Panel::create();
  spriteBrushOptionsPanel_->setSize("50%", "40%");
  spriteBrushOptionsPanel_->setPosition("34%", "5%");
  spriteBrushOptionsPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  spriteBrushOptionsPanel_->getRenderer()->setBorders({1});
  spriteBrushOptionsPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  spriteBrushOptionsPanel_->getRenderer()->setRoundedBorderRadius(8);
  spriteBrushOptionsPanel_->setVisible(false);
  gui.add(spriteBrushOptionsPanel_);

  // Maintenant, je crée les différents tabs (pages) pour catégoriser les images
  auto tabNature = tgui::Button::create("Nature");
  auto tabConstruct = tgui::Button::create("Construction");
  auto tabObjects = tgui::Button::create("Objects");
  auto tabImport = tgui::Button::create("Perso");

  // Je configure les pages
  tabNature->setSize("25%", "10%");
  tabNature->setPosition("0%", "0%");
  tabNature->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
  tabNature->getRenderer()->setTextColor(tgui::Color::White);
  tabNature->getRenderer()->setBorders({0});
  tabNature->setTextSize(0);
  tabNature->onPress([this, tabNature, tabConstruct, tabObjects, tabImport]() {
    natureSpritePanel_->setVisible(true);
    constructSpritePanel_->setVisible(false);
    objectsSpritePanel_->setVisible(false);
    importPanel_->setVisible(false);
    tabNature->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    tabConstruct->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabObjects->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabImport->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  });
  spriteBrushOptionsPanel_->add(tabNature);

  tabConstruct->setSize("25%", "10%");
  tabConstruct->setPosition("25%", "0%");
  tabConstruct->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  tabConstruct->getRenderer()->setTextColor(tgui::Color::White);
  tabConstruct->getRenderer()->setBorders({0});
  tabConstruct->setTextSize(0);
  tabConstruct->onPress([this, tabNature, tabConstruct, tabObjects, tabImport]() {
    natureSpritePanel_->setVisible(false);
    constructSpritePanel_->setVisible(true);
    objectsSpritePanel_->setVisible(false);
    importPanel_->setVisible(false);
    tabNature->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabConstruct->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    tabObjects->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabImport->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  });
  spriteBrushOptionsPanel_->add(tabConstruct);

  tabObjects->setSize("25%", "10%");
  tabObjects->setPosition("50%", "0%");
  tabObjects->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  tabObjects->getRenderer()->setTextColor(tgui::Color::White);
  tabObjects->getRenderer()->setBorders({0});
  tabObjects->setTextSize(0);
  tabObjects->onPress([this, tabNature, tabConstruct, tabObjects, tabImport]() {
    natureSpritePanel_->setVisible(false);
    constructSpritePanel_->setVisible(false);
    objectsSpritePanel_->setVisible(true);
    importPanel_->setVisible(false);
    tabNature->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabConstruct->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabObjects->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    tabImport->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  });
  spriteBrushOptionsPanel_->add(tabObjects);

  tabImport->setSize("25%", "10%");
  tabImport->setPosition("75%", "0%");
  tabImport->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  tabImport->getRenderer()->setTextColor(tgui::Color::White);
  tabImport->getRenderer()->setBorders({0});
  tabImport->setTextSize(0);
  tabImport->onPress([this, tabNature, tabConstruct, tabObjects, tabImport]() {
    natureSpritePanel_->setVisible(false);
    constructSpritePanel_->setVisible(false);
    objectsSpritePanel_->setVisible(false);
    importPanel_->setVisible(true);
    tabNature->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabConstruct->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabObjects->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabImport->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    refreshImportPanel();
  });
  spriteBrushOptionsPanel_->add(tabImport);

  // Ici, je stocke toutes les images sélectionner afin de les utiliser avec le sprite brush
  spritesSelected_ = std::make_shared<std::vector<std::string>>();

  // Maintenant, je crée les pannels qui vont affciher les srites
  natureSpritePanel_ = tgui::ScrollablePanel::create();
  natureSpritePanel_->setSize("100%", "90%");
  natureSpritePanel_->setPosition("0%", "10%");
  natureSpritePanel_->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  natureSpritePanel_->getRenderer()->setBorders({0});
  natureSpritePanel_->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Automatic);
  natureSpritePanel_->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  spriteBrushOptionsPanel_->add(natureSpritePanel_);

  constructSpritePanel_ = tgui::ScrollablePanel::create();
  constructSpritePanel_->setSize("100%", "90%");
  constructSpritePanel_->setPosition("0%", "10%");
  constructSpritePanel_->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  constructSpritePanel_->getRenderer()->setBorders({0});
  constructSpritePanel_->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Automatic);
  constructSpritePanel_->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  constructSpritePanel_->setVisible(false);
  spriteBrushOptionsPanel_->add(constructSpritePanel_);

  objectsSpritePanel_ = tgui::ScrollablePanel::create();
  objectsSpritePanel_->setSize("100%", "90%");
  objectsSpritePanel_->setPosition("0%", "10%");
  objectsSpritePanel_->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  objectsSpritePanel_->getRenderer()->setBorders({0});
  objectsSpritePanel_->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Automatic);
  objectsSpritePanel_->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  objectsSpritePanel_->setVisible(false);
  spriteBrushOptionsPanel_->add(objectsSpritePanel_);

  auto panelImport = tgui::ScrollablePanel::create();
  panelImport->setSize("100%", "90%");
  panelImport->setPosition("0%", "10%");
  panelImport->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  panelImport->getRenderer()->setBorders({0});
  panelImport->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Automatic);
  panelImport->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);

  // Je crée le bouton qui va permettre d'importer des images
  auto importButton = tgui::Button::create("Importer");
  importButton->setSize("40%", "12%");
  importButton->setPosition("30%", "2%");
  importButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
  importButton->getRenderer()->setTextColor(tgui::Color::White);
  importButton->getRenderer()->setBorders({0});
  importButton->setTextSize(14);
  importButton->onPress([this]() {
    auto result = pfd::open_file("Choisissez votre image", ".", { "Images PNG", "*.png"}).result();
    if (!result.empty()) {
        sf::Texture texture;
        if (texture.loadFromFile(result[0]))
            app_.getNetwork().addSprite(texture);
    }
  });
  panelImport->add(importButton, "importBtn");
  importPanel_ = panelImport;
  panelImport->setVisible(false);
  spriteBrushOptionsPanel_->add(panelImport);

  // On affiche les images, je le fait pas qu'une fois car il faut adapter l'affichages des images 
  // à chaque fois que la fenêtre est redimentionné
  refreshSpriteBrushOptions();
  spriteBrushOptionsPanel_->onSizeChange([this]() { refreshSpriteBrushOptions(); });
}

void GameView::refreshNatureSpritePanel() {
  // Si on est pas dans la page nature, pas besoin de la recréer
  if (!natureSpritePanel_) return;

  natureSpritePanel_->removeAllWidgets();
  float imageSize = spriteBrushOptionsPanel_->getSize().x / 6.f;
  int column = 0, row = 0;
  
  // On crée les images à afficher une à une
  for (auto& entry : project->getMap()->getAssetManager().getAllAssets()) {
    std::string id = entry.first;
    auto& asset = entry.second;

    if (asset.category == "nature") {
      // On regarde si l'image est sélectionné, pour changer son affichage
      bool selected = false;
      for (auto currentId : *spritesSelected_) {
          if (currentId == id) {
              selected = true;
              break;
          }
      }

      // On crée l'image sous forme de bouton
      auto image = tgui::Button::create();
      image->setSize(imageSize, imageSize);
      image->setPosition(column * imageSize, row * imageSize);
      image->getRenderer()->setTexture(tgui::Texture("../res/sprites/" + asset.filename));
      image->getRenderer()->setBorders({3});
      image->getRenderer()->setBorderColor(selected ? tgui::Color(0, 120, 255) : tgui::Color::Transparent);
      image->getRenderer()->setBorderColorHover(tgui::Color(0, 120, 255));
      image->getRenderer()->setBorderColorDown(tgui::Color(0, 80, 200));
      image->onClick([this, image, id]() {
        auto tool = std::dynamic_pointer_cast<SpriteBrush>(project->getToolBar().getSelectedTool());
        if (!tool) {
          project->getToolBar().selectTool(SPRITEBRUSH);
          tool = std::dynamic_pointer_cast<SpriteBrush>(project->getToolBar().getSelectedTool());
        }
        if (!tool) return;
        auto it = std::find(spritesSelected_->begin(), spritesSelected_->end(), id);
        if (it != spritesSelected_->end()) {
          spritesSelected_->erase(it);
          image->getRenderer()->setBorderColor(tgui::Color::Transparent);
          tool->removeAsset(id);
        } else {
          spritesSelected_->push_back(id);
          image->getRenderer()->setBorderColor(tgui::Color(0, 120, 255));
          tool->addAsset(id);
        }
      });
      natureSpritePanel_->add(image);

      column++;
      if (column >= 6) { column = 0; row++; }
    }
  }
}

void GameView::refreshConstructSpritePanel() {
  // Si on est pas dans la page construction, pas besoin de la recréer
  if (!constructSpritePanel_) return;

  constructSpritePanel_->removeAllWidgets();
  float imageSize = spriteBrushOptionsPanel_->getSize().x / 6.f;
  int column = 0, row = 0;

  // On crée les images à afficher une à une
  for (auto& entry : project->getMap()->getAssetManager().getAllAssets()) {
    std::string id = entry.first;
    auto& asset = entry.second;

    if (asset.category == "building") {
      // On regarde si l'image est sélectionné, pour changer son affichage
      bool selected = false;
      for (auto currentId : *spritesSelected_) {
          if (currentId == id) {
              selected = true;
              break;
          }
      }

      // On crée l'image sous forme de bouton
      auto image = tgui::Button::create();
      image->setSize(imageSize, imageSize);
      image->setPosition(column * imageSize, row * imageSize);
      image->getRenderer()->setTexture(tgui::Texture("../res/sprites/" + asset.filename));
      image->getRenderer()->setBorders({3});
      image->getRenderer()->setBorderColor(selected ? tgui::Color(0, 120, 255) : tgui::Color::Transparent);
      image->getRenderer()->setBorderColorHover(tgui::Color(0, 120, 255));
      image->getRenderer()->setBorderColorDown(tgui::Color(0, 80, 200));
      image->onClick([this, image, id]() {
        auto tool = std::dynamic_pointer_cast<SpriteBrush>(project->getToolBar().getSelectedTool());
        if (!tool) {
          project->getToolBar().selectTool(SPRITEBRUSH);
          tool = std::dynamic_pointer_cast<SpriteBrush>(project->getToolBar().getSelectedTool());
        }
        if (!tool) return;
        auto it = std::find(spritesSelected_->begin(), spritesSelected_->end(), id);
        if (it != spritesSelected_->end()) {
          spritesSelected_->erase(it);
          image->getRenderer()->setBorderColor(tgui::Color::Transparent);
          tool->removeAsset(id);
        } else {
          spritesSelected_->push_back(id);
          image->getRenderer()->setBorderColor(tgui::Color(0, 120, 255));
          tool->addAsset(id);
        }
      });
      constructSpritePanel_->add(image);

      column++;
      if (column >= 6) { column = 0; row++; }
    }
  }
}

void GameView::refreshObjectsSpritePanel() {
  // Si on est pas dans la page objects, pas besoin de la recréer
  if (!objectsSpritePanel_) return;

  objectsSpritePanel_->removeAllWidgets();
  float imageSize = spriteBrushOptionsPanel_->getSize().x / 6.f;
  int column = 0, row = 0;

  // On crée les images à afficher une à une
  for (auto& entry : project->getMap()->getAssetManager().getAllAssets()) {
    std::string id = entry.first;
    auto& asset = entry.second;

    if (asset.category == "object") {
      // On regarde si l'image est sélectionné, pour changer son affichage
      bool selected = false;
      for (auto currentId : *spritesSelected_) {
          if (currentId == id) {
              selected = true;
              break;
          }
      }

      // On crée l'image sous forme de bouton
      auto image = tgui::Button::create();
      image->setSize(imageSize, imageSize);
      image->setPosition(column * imageSize, row * imageSize);
      image->getRenderer()->setTexture(tgui::Texture("../res/sprites/" + asset.filename));
      image->getRenderer()->setBorders({3});
      image->getRenderer()->setBorderColor(selected ? tgui::Color(0, 120, 255) : tgui::Color::Transparent);
      image->getRenderer()->setBorderColorHover(tgui::Color(0, 120, 255));
      image->getRenderer()->setBorderColorDown(tgui::Color(0, 80, 200));
      image->onClick([this, image, id]() {
        auto tool = std::dynamic_pointer_cast<SpriteBrush>(project->getToolBar().getSelectedTool());
        if (!tool) {
          project->getToolBar().selectTool(SPRITEBRUSH);
          tool = std::dynamic_pointer_cast<SpriteBrush>(project->getToolBar().getSelectedTool());
        }
        if (!tool) return;
        auto it = std::find(spritesSelected_->begin(), spritesSelected_->end(), id);
        if (it != spritesSelected_->end()) {
          spritesSelected_->erase(it);
          image->getRenderer()->setBorderColor(tgui::Color::Transparent);
          tool->removeAsset(id);
        } else {
          spritesSelected_->push_back(id);
          image->getRenderer()->setBorderColor(tgui::Color(0, 120, 255));
          tool->addAsset(id);
        }
      });
      objectsSpritePanel_->add(image);

      column++;
      if (column >= 6) { column = 0; row++; }
    }
  }
}

void GameView::refreshSpriteBrushOptions() {
  if (!natureSpritePanel_) return;
  refreshNatureSpritePanel();
  refreshConstructSpritePanel();
  refreshObjectsSpritePanel();
  if (importPanel_ && importPanel_->isVisible())
    refreshImportPanel();
}

void GameView::refreshImportPanel() {
  // Si on est pas dans la page import, pas besoin de la recréer
  if (!importPanel_) return;

  // On sauvegarde le bouton d'import avant de tout effacer
  auto importBtn = importPanel_->get<tgui::Button>("importBtn");
  importPanel_->removeAllWidgets();
  importPanel_->add(importBtn, "importBtn");

  float imageSize = spriteBrushOptionsPanel_->getSize().x / 6.f;
  // On décale les images vers le bas pour laisser la place au bouton d'import
  float yOffset = spriteBrushOptionsPanel_->getSize().y * 0.90f * 0.15f;
  int column = 0, row = 0;

  // On crée les images à afficher une à une
  for (auto& entry : project->getMap()->getAssetManager().getAllAssets()) {
    std::string id = entry.first;
    auto& asset = entry.second;

    if (asset.category == "import") {
      // On regarde si l'image est sélectionné, pour changer son affichage
      bool selected = false;
      for (auto currentId : *spritesSelected_) {
          if (currentId == id) {
              selected = true;
              break;
          }
      }

      // On crée l'image sous forme de bouton
      // Les assets importés sont en mémoire, pas sur disque, donc on charge via pixel data
      tgui::Texture texture;
      sf::Image img = asset.texture->copyToImage();
      texture.loadFromPixelData(asset.texture->getSize(), img.getPixelsPtr());

      auto image = tgui::Button::create();
      image->setSize(imageSize, imageSize);
      image->setPosition(column * imageSize, yOffset + row * imageSize);
      image->getRenderer()->setTexture(texture);
      image->getRenderer()->setBorders({3});
      image->getRenderer()->setBorderColor(selected ? tgui::Color(0, 120, 255) : tgui::Color::Transparent);
      image->getRenderer()->setBorderColorHover(tgui::Color(0, 120, 255));
      image->getRenderer()->setBorderColorDown(tgui::Color(0, 80, 200));
      image->onClick([this, image, id]() {
        auto tool = std::dynamic_pointer_cast<SpriteBrush>(project->getToolBar().getSelectedTool());
        if (!tool) {
          project->getToolBar().selectTool(SPRITEBRUSH);
          tool = std::dynamic_pointer_cast<SpriteBrush>(project->getToolBar().getSelectedTool());
        }
        if (!tool) return;
        auto it = std::find(spritesSelected_->begin(), spritesSelected_->end(), id);
        if (it != spritesSelected_->end()) {
          spritesSelected_->erase(it);
          image->getRenderer()->setBorderColor(tgui::Color::Transparent);
          tool->removeAsset(id);
        } else {
          spritesSelected_->push_back(id);
          image->getRenderer()->setBorderColor(tgui::Color(0, 120, 255));
          tool->addAsset(id);
        }
      });
      importPanel_->add(image);

      column++;
      if (column >= 6) { column = 0; row++; }
    }
  }
}
