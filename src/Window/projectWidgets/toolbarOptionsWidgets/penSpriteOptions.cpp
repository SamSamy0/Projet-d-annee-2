#include "../../Application.hpp"
#include "../GameView.hpp"
#include "../../../project/Tool/spritebrush.hpp"

void GameView::initPenSpriteOptions() {
  auto& mainWindow = app_.getWindow();
  auto& gui = app_.getGui();
  float width  = mainWindow.getSize().x;
  float height = mainWindow.getSize().y;

  penSpriteOptionsPanel_ = tgui::Panel::create();
  penSpriteOptionsPanel_->setSize(width * 0.10, height * 0.062);
  penSpriteOptionsPanel_->setPosition(width * 0.22, height * 0.05);
  penSpriteOptionsPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  penSpriteOptionsPanel_->getRenderer()->setBorders({1});
  penSpriteOptionsPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  penSpriteOptionsPanel_->getRenderer()->setRoundedBorderRadius(8);
  penSpriteOptionsPanel_->setVisible(false);
  gui.add(penSpriteOptionsPanel_);

  auto widthInput = tgui::EditBox::create();
  widthInput->setSize(width * 0.04, height * 0.04);
  widthInput->setPosition(width * 0.008, height * 0.011);
  widthInput->setDefaultText("L");
  widthInput->setInputValidator("[0-9]+\\.?[0-9]*");
  widthInput->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  widthInput->getRenderer()->setTextColor(tgui::Color::White);
  widthInput->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
  widthInput->getRenderer()->setBorders({1});
  widthInput->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  penSpriteOptionsPanel_->add(widthInput);


  widthInput->onTextChange([this, widthInput]() {
    // regarde s'il y a aucune valeur
    if (widthInput->getText().empty()) return;
    
    // Ici il regarde que la taille de la longueur (x) et largeur (y) du pinceau soit au moins égale à 1, et si y pas de valeur on prend la valeur de x
    unsigned int sizeX = static_cast<unsigned int>(std::stoi(widthInput->getText().toStdString()));
    if (sizeX < 1) sizeX = 1;

    // On applique la nouvelle taille
    auto brush = dynamic_pointer_cast<SpriteBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setSize(sizeX,0);
  });

}
