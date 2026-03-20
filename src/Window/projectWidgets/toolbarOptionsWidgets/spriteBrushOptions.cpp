#include "../../Application.hpp"
#include "../GameView.hpp"
#include "../../../project/Tool/spritebrush.hpp"

void GameView::initSpriteBrushOptions() {
  auto& mainWindow = app_.getWindow();
  auto& gui = app_.getGui();
  float width  = mainWindow.getSize().x;
  float height = mainWindow.getSize().y;

  // Menu principale pour la sélection des images
  spriteBrushOptionsPanel_ = tgui::Panel::create();
  spriteBrushOptionsPanel_->setSize(width * 0.50, height * 0.4);
  spriteBrushOptionsPanel_->setPosition(width * 0.34, height * 0.05);
  spriteBrushOptionsPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  spriteBrushOptionsPanel_->getRenderer()->setBorders({1});
  spriteBrushOptionsPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  spriteBrushOptionsPanel_->getRenderer()->setRoundedBorderRadius(8);
  spriteBrushOptionsPanel_->setVisible(false);
  gui.add(spriteBrushOptionsPanel_);

  // Liste scrollable comprenant l'ensemble des images du projet
  auto scrollPanel = tgui::ScrollablePanel::create();
  scrollPanel->setSize(width * 0.50, height * 0.4);
  scrollPanel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  scrollPanel->getRenderer()->setBorders({0});
  scrollPanel->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  scrollPanel->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  spriteBrushOptionsPanel_->add(scrollPanel);

  const auto& allSprites = project->getMap()->getAssetManager().getAllAssets();
  float imageSize = spriteBrushOptionsPanel_->getSize().x / 6;
  auto imagesSelected = std::make_shared<std::vector<std::string>>();
  int column = 0;
  int row = 0;

  for (const auto& [id, asset] : allSprites) {
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
          auto tool = std::dynamic_pointer_cast<SpriteBrush>(project->getToolBar().getSelectedTool());
          if (!tool) {
              project->getToolBar().selectTool(SPRITEBRUSH);
              tool = std::dynamic_pointer_cast<SpriteBrush>(project->getToolBar().getSelectedTool());
          }
          if (!tool) return;

          // Je regarde si l'image a déjà été sélectionner
          bool found = false;
          for (auto& img : *imagesSelected) {
              if (img == assetId) {
                  found = true;
                  break;
              }
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

      scrollPanel->add(image);
      column++;
      if (column % 6 == 0) row++;
  }
}