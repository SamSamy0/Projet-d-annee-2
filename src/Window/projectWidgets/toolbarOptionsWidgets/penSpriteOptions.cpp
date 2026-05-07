#include "../../Application.hpp"
#include "../GameView.hpp"
#include "../../../project/Tool/spritebrush.hpp"

void GameView::initPenSpriteOptions() {
  auto& gui = app_.getGui();

  penSpriteOptionsPanel_ = tgui::Panel::create();
  penSpriteOptionsPanel_->setSize("15%", "6.2%");
  penSpriteOptionsPanel_->setPosition("22%", "5%");
  penSpriteOptionsPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  penSpriteOptionsPanel_->getRenderer()->setBorders({1});
  penSpriteOptionsPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  penSpriteOptionsPanel_->getRenderer()->setRoundedBorderRadius(8);
  penSpriteOptionsPanel_->setVisible(false);
  gui.add(penSpriteOptionsPanel_);

  auto widthInput = tgui::EditBox::create();
  widthInput->setSize("26.7%", "64%");
  widthInput->setPosition("5.3%", "18%");
  widthInput->setDefaultText("L");
  widthInput->setInputValidator("[0-9]*\\.?[0-9]*");
  widthInput->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  widthInput->getRenderer()->setTextColor(tgui::Color::White);
  widthInput->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
  widthInput->getRenderer()->setBorders({1});
  widthInput->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  penSpriteOptionsPanel_->add(widthInput);

  auto spaceInput = tgui::EditBox::create();
  spaceInput->setSize("46.7%", "64%");
  spaceInput->setPosition("35%", "18%");
  spaceInput->setDefaultText("Offset");
  spaceInput->setInputValidator("-?[0-9]*\\.?[0-9]*");
  spaceInput->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  spaceInput->getRenderer()->setTextColor(tgui::Color::White);
  spaceInput->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
  spaceInput->getRenderer()->setBorders({1});
  spaceInput->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  penSpriteOptionsPanel_->add(spaceInput);

  spaceInput->onTextChange([this, spaceInput]() {

    if (spaceInput->getText().empty()) return;
    if (spaceInput->getText().empty() || spaceInput->getText() == "-" || spaceInput->getText() == "." || spaceInput->getText() == "-.")
        return;
    float offset = std::stof(spaceInput->getText().toStdString());
    auto brush = dynamic_pointer_cast<SpriteBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setOffset(offset);
  });

  widthInput->onTextChange([this, widthInput]() {

    if (widthInput->getText().empty()) return;

    float sizeX = std::stof(widthInput->getText().toStdString());
    // if (sizeX < 1) sizeX = 1;

    // On applique la nouvelle taille
    auto brush = dynamic_pointer_cast<SpriteBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setSize(sizeX,0);
  });

}
