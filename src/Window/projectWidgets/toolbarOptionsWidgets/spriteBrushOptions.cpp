#include "../../Window.hpp"
#include "../../../project/Tool/spritebrush.hpp"

void Window::initSpriteBrushOptions() {
  float width  = mainWindow.getSize().x;
  float height = mainWindow.getSize().y;

  // Panneau principal
  spriteBrushOptionsPanel_ = tgui::Panel::create();
  spriteBrushOptionsPanel_->setSize(width * 0.50, height * 0.4);
  spriteBrushOptionsPanel_->setPosition(width * 0.34, height * 0.05);
  spriteBrushOptionsPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  spriteBrushOptionsPanel_->getRenderer()->setBorders({1});
  spriteBrushOptionsPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  spriteBrushOptionsPanel_->getRenderer()->setRoundedBorderRadius(8);
  spriteBrushOptionsPanel_->setVisible(false);
  gui.add(spriteBrushOptionsPanel_);

  auto scrollPanel = tgui::ScrollablePanel::create();
  scrollPanel->setSize("100%", "100%");
  scrollPanel->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  scrollPanel->getRenderer()->setBorders({0});
  scrollPanel->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  scrollPanel->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  spriteBrushOptionsPanel_->add(scrollPanel);

  const auto& allSprites = project->getMap()->getAssetManager().getAllAssets();
  float imageSize = spriteBrushOptionsPanel_->getSize().x * 0.15;
  auto selection = std::make_shared<std::vector<std::string>>();
  int column = 0;
  int row = 0;

  for (const auto& [id, asset] : allSprites)
  {
      column %= 5;
      float x = column * (imageSize + 10);
      float y = row * (imageSize + 10);

      auto image = tgui::Button::create();
      image->setSize(imageSize, imageSize);
      image->setPosition(x, y);
      image->getRenderer()->setTexture(tgui::Texture("../res/sprites/" + asset.filename));
      image->getRenderer()->setBorders({3});
      image->getRenderer()->setBorderColor(tgui::Color::Transparent);
      image->getRenderer()->setBorderColorHover(tgui::Color(0, 120, 255));
      image->getRenderer()->setBorderColorDown(tgui::Color(0, 80, 200));

      auto assetId = id;

      image->onClick([this, image, assetId, selection]() {
          auto tool = std::dynamic_pointer_cast<SpriteBrush>(project->getToolBar().getSelectedTool());
          if (!tool) return;
          bool found = false;
          for (auto& s : *selection) {
              if (s == assetId) {
                  found = true;
                  break;
              }
          }
          if (found) {
              selection->erase(std::remove(selection->begin(), selection->end(), assetId), selection->end());
              image->getRenderer()->setBorderColor(tgui::Color::Transparent);
              tool->removeAsset(assetId);
          } else {
              selection->push_back(assetId);
              image->getRenderer()->setBorderColor(tgui::Color(0, 120, 255));
              tool->addAsset(assetId);
          }
      });

      scrollPanel->add(image);
      column++;
      if (column % 5 == 0) row++;
  }
}