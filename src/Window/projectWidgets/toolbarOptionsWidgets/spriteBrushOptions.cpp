#include "../../Application.hpp"
#include "../GameView.hpp"
#include "../../../project/Tool/spritebrush.hpp"
#include "../../../project/Tool/autofill.hpp"
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
  auto tabConstruction = tgui::Button::create("Construction");
  auto tabObjects = tgui::Button::create("Objects");
  auto tabImport = tgui::Button::create("Perso");

  // Je configure les pages
  tabNature->setSize("25%", "10%");
  tabNature->setPosition("0%", "0%");
  tabNature->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
  tabNature->getRenderer()->setTextColor(tgui::Color::White);
  tabNature->getRenderer()->setBorders({0});
  tabNature->setTextSize(0);
  tabNature->onPress([this, tabNature, tabConstruction, tabObjects, tabImport]() {
    panelNature_->setVisible(true);
    panelConstruction_->setVisible(false);
    panelObjects_->setVisible(false);
    panelImport_->setVisible(false);
    tabNature->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    tabConstruction->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabObjects->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabImport->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  });
  spriteBrushOptionsPanel_->add(tabNature);

  tabConstruction->setSize("25%", "10%");
  tabConstruction->setPosition("25%", "0%");
  tabConstruction->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  tabConstruction->getRenderer()->setTextColor(tgui::Color::White);
  tabConstruction->getRenderer()->setBorders({0});
  tabConstruction->setTextSize(0);
  tabConstruction->onPress([this, tabNature, tabConstruction, tabObjects, tabImport]() {
    panelNature_->setVisible(false);
    panelConstruction_->setVisible(true);
    panelObjects_->setVisible(false);
    panelImport_->setVisible(false);
    tabNature->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabConstruction->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    tabObjects->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabImport->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  });
  spriteBrushOptionsPanel_->add(tabConstruction);

  tabObjects->setSize("25%", "10%");
  tabObjects->setPosition("50%", "0%");
  tabObjects->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  tabObjects->getRenderer()->setTextColor(tgui::Color::White);
  tabObjects->getRenderer()->setBorders({0});
  tabObjects->setTextSize(0);
  tabObjects->onPress([this, tabNature, tabConstruction, tabObjects, tabImport]() {
    panelNature_->setVisible(false);
    panelConstruction_->setVisible(false);
    panelObjects_->setVisible(true);
    panelImport_->setVisible(false);
    tabNature->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabConstruction->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
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
  tabImport->onPress([this, tabNature, tabConstruction, tabObjects, tabImport]() {
    panelNature_->setVisible(false);
    panelConstruction_->setVisible(false);
    panelObjects_->setVisible(false);
    panelImport_->setVisible(true);
    tabNature->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabConstruction->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabObjects->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabImport->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    refreshImportPanel();
  });
  spriteBrushOptionsPanel_->add(tabImport);

  // Ici, je stocke toutes les images sélectionner afin de les utiliser avec le sprite brush
  spritesSelected_ = std::make_shared<std::vector<std::string>>();

  // Maintenant, je crée les pannels qui vont affciher les srites
  panelNature_ = tgui::ScrollablePanel::create();
  panelNature_->setSize("100%", "90%");
  panelNature_->setPosition("0%", "10%");
  panelNature_->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  panelNature_->getRenderer()->setBorders({0});
  panelNature_->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Automatic);
  panelNature_->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  spriteBrushOptionsPanel_->add(panelNature_);

  panelConstruction_ = tgui::ScrollablePanel::create();
  panelConstruction_->setSize("100%", "90%");
  panelConstruction_->setPosition("0%", "10%");
  panelConstruction_->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  panelConstruction_->getRenderer()->setBorders({0});
  panelConstruction_->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Automatic);
  panelConstruction_->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  panelConstruction_->setVisible(false);
  spriteBrushOptionsPanel_->add(panelConstruction_);

  panelObjects_ = tgui::ScrollablePanel::create();
  panelObjects_->setSize("100%", "90%");
  panelObjects_->setPosition("0%", "10%");
  panelObjects_->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  panelObjects_->getRenderer()->setBorders({0});
  panelObjects_->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Automatic);
  panelObjects_->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  panelObjects_->setVisible(false);
  spriteBrushOptionsPanel_->add(panelObjects_);

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
  panelImport_ = panelImport;
  panelImport->setVisible(false);
  spriteBrushOptionsPanel_->add(panelImport);

  // On affiche les images, je le fait pas qu'une fois car il faut adapter l'affichages des images 
  // à chaque fois que la fenêtre est redimentionné
  refreshSpriteBrushOptions();
  spriteBrushOptionsPanel_->onSizeChange([this]() { refreshSpriteBrushOptions(); });
}

void GameView::refreshNatureSpritePanel() {
  // Si on est pas dans la page nature, pas besoin de la recréer
  if (!panelNature_) return;

  panelNature_->removeAllWidgets();
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
      panelNature_->add(image);

      column++;
      if (column >= 6) { column = 0; row++; }
    }
  }
}

void GameView::refreshConstructSpritePanel() {
  // Si on est pas dans la page construction, pas besoin de la recréer
  if (!panelConstruction_) return;

  panelConstruction_->removeAllWidgets();
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
      panelConstruction_->add(image);

      column++;
      if (column >= 6) { column = 0; row++; }
    }
  }
}

void GameView::refreshObjectsSpritePanel() {
  // Si on est pas dans la page objects, pas besoin de la recréer
  if (!panelObjects_) return;

  panelObjects_->removeAllWidgets();
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
      panelObjects_->add(image);

      column++;
      if (column >= 6) { column = 0; row++; }
    }
  }
}

void GameView::refreshSpriteBrushOptions() {
  if (!panelNature_) return;
  refreshNatureSpritePanel();
  refreshConstructSpritePanel();
  refreshObjectsSpritePanel();
  if (panelImport_ && panelImport_->isVisible())
    refreshImportPanel();
}

void GameView::refreshImportPanel() {
  // Si on est pas dans la page import, pas besoin de la recréer
  if (!panelImport_) return;

  // On sauvegarde le bouton d'import avant de tout effacer
  auto importBtn = panelImport_->get<tgui::Button>("importBtn");
  panelImport_->removeAllWidgets();
  panelImport_->add(importBtn, "importBtn");

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
      panelImport_->add(image);

      column++;
      if (column >= 6) { column = 0; row++; }
    }
  }
}
