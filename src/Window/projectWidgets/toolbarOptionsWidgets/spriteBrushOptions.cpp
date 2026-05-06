#include "../../Application.hpp"
#include "../GameView.hpp"
#include "../../../project/Tool/spritebrush.hpp"
#include "../../../project/Tool/autofill.hpp"
#include <portable-file-dialogs.h>

void GameView::initSpriteBrushOptions() {
  auto& mainWindow = app_.getWindow();
  auto& gui = app_.getGui();

  spriteBrushOptionsPanel_ = tgui::Panel::create();
  spriteBrushOptionsPanel_->setSize("50%", "40%");
  spriteBrushOptionsPanel_->setPosition("48%", "5%");
  spriteBrushOptionsPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  spriteBrushOptionsPanel_->getRenderer()->setBorders({1});
  spriteBrushOptionsPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  spriteBrushOptionsPanel_->getRenderer()->setRoundedBorderRadius(8);
  spriteBrushOptionsPanel_->setVisible(false);
  gui.add(spriteBrushOptionsPanel_);

  auto tabNature = tgui::Button::create("Nature");
  auto tabConstruct = tgui::Button::create("Construction");
  auto tabObjects = tgui::Button::create("Objects");
  auto tabImport = tgui::Button::create("Perso");

  tabNature->setSize("25%", "10%");
  tabNature->setPosition("0%", "0%");
  tabNature->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
  tabNature->getRenderer()->setTextColor(tgui::Color::White);
  tabNature->getRenderer()->setBorders({0});
  tabNature->setTextSize(0);
  spriteBrushOptionsPanel_->add(tabNature);

  tabConstruct->setSize("25%", "10%");
  tabConstruct->setPosition("25%", "0%");
  tabConstruct->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  tabConstruct->getRenderer()->setTextColor(tgui::Color::White);
  tabConstruct->getRenderer()->setBorders({0});
  tabConstruct->setTextSize(0);
  spriteBrushOptionsPanel_->add(tabConstruct);

  tabObjects->setSize("25%", "10%");
  tabObjects->setPosition("50%", "0%");
  tabObjects->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  tabObjects->getRenderer()->setTextColor(tgui::Color::White);
  tabObjects->getRenderer()->setBorders({0});
  tabObjects->setTextSize(0);
  spriteBrushOptionsPanel_->add(tabObjects);

  tabImport->setSize("25%", "10%");
  tabImport->setPosition("75%", "0%");
  tabImport->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  tabImport->getRenderer()->setTextColor(tgui::Color::White);
  tabImport->getRenderer()->setBorders({0});
  tabImport->setTextSize(0);
  spriteBrushOptionsPanel_->add(tabImport);

  const auto& allSprites = project->getMap()->getAssetManager().getAllAssets();
  float imageSize = mainWindow.getSize().x * 0.50f / 6;
  auto imagesSelected = std::make_shared<std::vector<std::string>>();
  int column = 0, row = 0;

  // onglet nature
  auto panelNature = tgui::Panel::create();
  panelNature->setSize("100%", "90%");
  panelNature->setPosition("0%", "10%");
  panelNature->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  panelNature->getRenderer()->setBorders({0});
  column = 0; row = 0;
  for (auto& entry : allSprites) {
      std::string id = entry.first;
      auto& asset = entry.second;
      if (asset.category != "nature") continue;
      column %= 6;
      auto image = tgui::Button::create();
      image->setSize(imageSize, imageSize);
      image->setPosition(column * imageSize, row * imageSize);
      image->getRenderer()->setTexture(tgui::Texture("../res/sprites/" + asset.filename));
      image->getRenderer()->setBorders({3});
      image->getRenderer()->setBorderColor(tgui::Color::Transparent);
      image->getRenderer()->setBorderColorHover(tgui::Color(0, 120, 255));
      image->getRenderer()->setBorderColorDown(tgui::Color(0, 80, 200));
      auto assetId = id;
      image->onClick([this, image, assetId, imagesSelected]() {
        auto selectedTool = project->getToolBar().getSelectedTool();
        auto spriteBrush = std::dynamic_pointer_cast<SpriteBrush>(selectedTool);
        auto autoFill = std::dynamic_pointer_cast<AutoFill>(selectedTool);

        if (!spriteBrush && !autoFill) return;

        bool found = false;
        for (auto& img : *imagesSelected) {
          if (img == assetId) { found = true; break; }
        }
        if (found) {
          imagesSelected->erase(std::remove(imagesSelected->begin(), imagesSelected->end(), assetId), imagesSelected->end());
          image->getRenderer()->setBorderColor(tgui::Color::Transparent);
          if (spriteBrush) spriteBrush->removeAsset(assetId);
          if (autoFill) autoFill->removeAsset(assetId);
        } else {
          imagesSelected->push_back(assetId);
          image->getRenderer()->setBorderColor(tgui::Color(0, 120, 255));
          if (spriteBrush) spriteBrush->addAsset(assetId);
          if (autoFill) autoFill->addAsset(assetId);
        }
      });
      panelNature->add(image);
      column++;
      if (column % 6 == 0) row++;
  }
  spriteBrushOptionsPanel_->add(panelNature);

  // onglet construction
  auto panelConstruct = tgui::Panel::create();
  panelConstruct->setSize("100%", "90%");
  panelConstruct->setPosition("0%", "10%");
  panelConstruct->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  panelConstruct->getRenderer()->setBorders({0});
  column = 0; row = 0;
  for (auto& entry : allSprites) {
      std::string id = entry.first;
      auto& asset = entry.second;
      if (asset.category != "building") continue;
      column %= 6;
      auto image = tgui::Button::create();
      image->setSize(imageSize, imageSize);
      image->setPosition(column * imageSize, row * imageSize);
      image->getRenderer()->setTexture(tgui::Texture("../res/sprites/" + asset.filename));
      image->getRenderer()->setBorders({3});
      image->getRenderer()->setBorderColor(tgui::Color::Transparent);
      image->getRenderer()->setBorderColorHover(tgui::Color(0, 120, 255));
      image->getRenderer()->setBorderColorDown(tgui::Color(0, 80, 200));
      auto assetId = id;
      image->onClick([this, image, assetId, imagesSelected]() {
        auto selectedTool = project->getToolBar().getSelectedTool();
        auto spriteBrush = std::dynamic_pointer_cast<SpriteBrush>(selectedTool);
        auto autoFill = std::dynamic_pointer_cast<AutoFill>(selectedTool);

        if (!spriteBrush && !autoFill) return;

        bool found = false;
        for (auto& img : *imagesSelected) {
          if (img == assetId) { found = true; break; }
        }
        if (found) {
          imagesSelected->erase(std::remove(imagesSelected->begin(), imagesSelected->end(), assetId), imagesSelected->end());
          image->getRenderer()->setBorderColor(tgui::Color::Transparent);
          if (spriteBrush) spriteBrush->removeAsset(assetId);
          if (autoFill) autoFill->removeAsset(assetId);
        } else {
          imagesSelected->push_back(assetId);
          image->getRenderer()->setBorderColor(tgui::Color(0, 120, 255));
          if (spriteBrush) spriteBrush->addAsset(assetId);
          if (autoFill) autoFill->addAsset(assetId);
        }
      });
      panelConstruct->add(image);
      column++;
      if (column % 6 == 0) row++;
  }
  panelConstruct->setVisible(false);
  spriteBrushOptionsPanel_->add(panelConstruct);

  // onglet objects
  auto panelObjects = tgui::Panel::create();
  panelObjects->setSize("100%", "90%");
  panelObjects->setPosition("0%", "10%");
  panelObjects->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  panelObjects->getRenderer()->setBorders({0});
  column = 0; row = 0;
  for (const auto& [id, asset] : allSprites) {
      if (asset.category != "object") continue;
      column %= 6;
      auto image = tgui::Button::create();
      image->setSize(imageSize, imageSize);
      image->setPosition(column * imageSize, row * imageSize);
      image->getRenderer()->setTexture(tgui::Texture("../res/sprites/" + asset.filename));
      image->getRenderer()->setBorders({3});
      image->getRenderer()->setBorderColor(tgui::Color::Transparent);
      image->getRenderer()->setBorderColorHover(tgui::Color(0, 120, 255));
      image->getRenderer()->setBorderColorDown(tgui::Color(0, 80, 200));
      auto assetId = id;
      image->onClick([this, image, assetId, imagesSelected]() {
        auto selectedTool = project->getToolBar().getSelectedTool();
        auto spriteBrush = std::dynamic_pointer_cast<SpriteBrush>(selectedTool);
        auto autoFill = std::dynamic_pointer_cast<AutoFill>(selectedTool);

        if (!spriteBrush && !autoFill) return;

        bool found = false;
        for (auto& img : *imagesSelected) {
          if (img == assetId) { found = true; break; }
        }
        if (found) {
          imagesSelected->erase(std::remove(imagesSelected->begin(), imagesSelected->end(), assetId), imagesSelected->end());
          image->getRenderer()->setBorderColor(tgui::Color::Transparent);
          if (spriteBrush) spriteBrush->removeAsset(assetId);
          if (autoFill) autoFill->removeAsset(assetId);
        } else {
          imagesSelected->push_back(assetId);
          image->getRenderer()->setBorderColor(tgui::Color(0, 120, 255));
          if (spriteBrush) spriteBrush->addAsset(assetId);
          if (autoFill) autoFill->addAsset(assetId);
        }
      });
      panelObjects->add(image);
      column++;
      if (column % 6 == 0) row++;
  }
  panelObjects->setVisible(false);
  spriteBrushOptionsPanel_->add(panelObjects);

  // onglet import
  auto panelImport = tgui::Panel::create();
  panelImport->setSize("100%", "90%");
  panelImport->setPosition("0%", "10%");
  panelImport->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  panelImport->getRenderer()->setBorders({0});

  auto importButton = tgui::Button::create("Importer");
  importButton->setSize("40%", "12%");
  importButton->setPosition("30%", "2%");
  importButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
  importButton->getRenderer()->setTextColor(tgui::Color::White);
  importButton->getRenderer()->setBorders({0});
  importButton->setTextSize(14);
  importButton->onPress([this]() {
    auto result = pfd::open_file(
        "Ouvrir un fichier",
        ".",
        { "Images PNG", "*.png" }
    ).result();

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

  tabNature->onPress([panelNature, panelConstruct, panelObjects, panelImport, tabNature, tabConstruct, tabObjects, tabImport]() {
    panelNature->setVisible(true);
    panelConstruct->setVisible(false);
    panelObjects->setVisible(false);
    panelImport->setVisible(false);
    tabNature->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    tabConstruct->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabObjects->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabImport->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  });
  tabConstruct->onPress([panelNature, panelConstruct, panelObjects, panelImport, tabNature, tabConstruct, tabObjects, tabImport]() {
    panelNature->setVisible(false);
    panelConstruct->setVisible(true);
    panelObjects->setVisible(false);
    panelImport->setVisible(false);
    tabNature->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabConstruct->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    tabObjects->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabImport->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  });
  tabObjects->onPress([panelNature, panelConstruct, panelObjects, panelImport, tabNature, tabConstruct, tabObjects, tabImport]() {
    panelNature->setVisible(false);
    panelConstruct->setVisible(false);
    panelObjects->setVisible(true);
    panelImport->setVisible(false);
    tabNature->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabConstruct->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabObjects->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    tabImport->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  });
  tabImport->onPress([this, panelNature, panelConstruct, panelObjects, panelImport, tabNature, tabConstruct, tabObjects, tabImport]() {
    panelNature->setVisible(false);
    panelConstruct->setVisible(false);
    panelObjects->setVisible(false);
    panelImport->setVisible(true);
    tabNature->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabConstruct->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabObjects->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    tabImport->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    refreshImportPanel();
  });
}

void GameView::refreshImportPanel() {
  if (!importPanel_) return;

  auto importBtn = importPanel_->get<tgui::Button>("importBtn");
  importPanel_->removeAllWidgets();
  importPanel_->add(importBtn, "importBtn");

  float imageSize = app_.getWindow().getSize().x * 0.50f / 6;
  float yOffset = app_.getWindow().getSize().y * 0.40f * 0.90f * 0.15f;
  auto imagesSelected = std::make_shared<std::vector<std::string>>();
  int col = 0, row = 0;

  for (auto& entry : project->getMap()->getAssetManager().getAllAssets()) {
    const auto& asset = entry.second;
    if (asset.category != "import") continue;
    col %= 6;
    auto image = tgui::Button::create();
    image->setSize(imageSize, imageSize);
    image->setPosition(col * imageSize, yOffset + row * imageSize);
    tgui::Texture tguiTex;
    sf::Image sfImg = asset.texture->copyToImage();
    tguiTex.loadFromPixelData(asset.texture->getSize(), sfImg.getPixelsPtr());
    image->getRenderer()->setTexture(tguiTex);
    image->getRenderer()->setBorders({3});
    image->getRenderer()->setBorderColor(tgui::Color::Transparent);
    image->getRenderer()->setBorderColorHover(tgui::Color(0, 120, 255));
    image->getRenderer()->setBorderColorDown(tgui::Color(0, 80, 200));
    auto assetId = entry.first;
    image->onClick([this, image, assetId, imagesSelected]() {
      auto tool = std::dynamic_pointer_cast<SpriteBrush>(project->getToolBar().getSelectedTool());
      if (!tool) {
        project->getToolBar().selectTool(SPRITEBRUSH);
        tool = std::dynamic_pointer_cast<SpriteBrush>(project->getToolBar().getSelectedTool());
      }
      if (!tool) return;
      bool found = false;
      for (auto& img : *imagesSelected) {
        if (img == assetId) { found = true; break; }
      }
      if (found) {
        imagesSelected->erase(std::remove(imagesSelected->begin(), imagesSelected->end(), assetId), imagesSelected->end());
        image->getRenderer()->setBorderColor(tgui::Color::Transparent);
        tool->removeAsset(assetId);
      } else {
        imagesSelected->push_back(assetId);
        image->getRenderer()->setBorderColor(tgui::Color(0, 120, 255));
        tool->addAsset(assetId);
      }
    });
    importPanel_->add(image);
    col++;
    if (col % 6 == 0) row++;
  }
}
