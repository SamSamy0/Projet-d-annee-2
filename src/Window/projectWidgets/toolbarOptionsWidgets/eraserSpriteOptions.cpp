#include "../../Application.hpp"
#include "../GameView.hpp"
#include "../../../project/Tool/spriteeraser.hpp"

void GameView::initEraserSpriteOptions() {
  auto& gui = app_.getGui();

  // Création de la barre avec toutes les infos pour la gomme sprite
  eraserSpriteOptionsPanel_ = tgui::Panel::create();
  eraserSpriteOptionsPanel_->setSize("35%", "6.2%");
  eraserSpriteOptionsPanel_->setPosition("34%", "5%");
  eraserSpriteOptionsPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  eraserSpriteOptionsPanel_->getRenderer()->setBorders({1});
  eraserSpriteOptionsPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  eraserSpriteOptionsPanel_->getRenderer()->setRoundedBorderRadius(8);
  eraserSpriteOptionsPanel_->setVisible(false);
  gui.add(eraserSpriteOptionsPanel_);

  auto widthInput = tgui::EditBox::create();
  widthInput->setSize("11.4%", "64%");
  widthInput->setPosition("2.3%", "18%");
  widthInput->setDefaultText("L");
  widthInput->setInputValidator("[0-9]+\\.?[0-9]*");
  widthInput->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  widthInput->getRenderer()->setTextColor(tgui::Color::White);
  widthInput->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
  widthInput->getRenderer()->setBorders({1});
  widthInput->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  eraserSpriteOptionsPanel_->add(widthInput);

  auto heightInput = tgui::EditBox::create();
  heightInput->setSize("11.4%", "64%");
  heightInput->setPosition("15%", "18%");
  heightInput->setDefaultText("l");
  heightInput->setInputValidator("[0-9]+\\.?[0-9]*");
  heightInput->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  heightInput->getRenderer()->setTextColor(tgui::Color::White);
  heightInput->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
  heightInput->getRenderer()->setBorders({1});
  heightInput->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  heightInput->setVisible(false);
  eraserSpriteOptionsPanel_->add(heightInput);

  widthInput->onTextChange([this, widthInput, heightInput]() {
    // regarde s'il y a aucune valeur
    if (widthInput->getText().empty()) return;

    // Ici il regarde que la taille de la longueur (x) et largeur (y) du pinceau soit au moins égale à 1, et si y pas de valeur on prend la valeur de x
    unsigned int sizeX = static_cast<unsigned int>(std::stoi(widthInput->getText().toStdString()));
    if (sizeX < 1) sizeX = 1;
    unsigned int sizeY = heightInput->getText().empty() ? sizeX : static_cast<unsigned int>(std::stoi(heightInput->getText().toStdString()));
    if (sizeY < 1) sizeY = 1;

    // On applique la nouvelle taille
    auto brush = dynamic_pointer_cast<SpriteEraser>(project->getToolBar().getSelectedTool());
    if (brush) brush->setSize(sizeX, sizeY);
  });
  heightInput->onTextChange([this, widthInput, heightInput]() {
    if (heightInput->getText().empty()) return;

    // Ici il regarde que la taille de la longueur (x) et largeur (y) du pinceau soit au moins égale à 1, et si y pas de valeur on prend la valeur de x
    unsigned int sizeX = widthInput->getText().empty() ? 1 : static_cast<unsigned int>(std::stoi(widthInput->getText().toStdString()));
    if (sizeX < 1) sizeX = 1;
    unsigned int sizeY = static_cast<unsigned int>(std::stoi(heightInput->getText().toStdString()));
    if (sizeY < 1) sizeY = 1;

    // On applique la nouvelle taille
    auto brush = dynamic_pointer_cast<SpriteEraser>(project->getToolBar().getSelectedTool());
    if (brush) brush->setSize(sizeX, sizeY);
  });

  // Création des différentes formes pour la gomme
  auto squareButton  = tgui::Button::create("Carré");
  auto diamondButton = tgui::Button::create("Diamant");
  auto circleButton  = tgui::Button::create("Cercle");

  // Carré
  squareButton->setSize("18.6%", "64%");
  squareButton->setPosition("29%", "18%");
  squareButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
  squareButton->getRenderer()->setTextColor(tgui::Color::White);
  squareButton->getRenderer()->setBorders({0});
  squareButton->getRenderer()->setRoundedBorderRadius(4);
  squareButton->onPress([this, squareButton, diamondButton, circleButton, heightInput]() {
    auto brush = dynamic_pointer_cast<SpriteEraser>(project->getToolBar().getSelectedTool());
    if (brush) brush->setShape(SQUARE);
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    heightInput->setVisible(false);
  });
  eraserSpriteOptionsPanel_->add(squareButton);
  squareButton->setTextSize(0);

  // Diamant (losange)
  diamondButton->setSize("18.6%", "64%");
  diamondButton->setPosition("49%", "18%");
  diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  diamondButton->getRenderer()->setTextColor(tgui::Color::White);
  diamondButton->getRenderer()->setBorders({0});
  diamondButton->getRenderer()->setRoundedBorderRadius(4);
  diamondButton->onPress([this, squareButton, diamondButton, circleButton, heightInput]() {
    auto brush = dynamic_pointer_cast<SpriteEraser>(project->getToolBar().getSelectedTool());
    if (brush) brush->setShape(DIAMOND);
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    heightInput->setVisible(true);
  });
  eraserSpriteOptionsPanel_->add(diamondButton);
  diamondButton->setTextSize(0);

  // Cercle
  circleButton->setSize("18.6%", "64%");
  circleButton->setPosition("70%", "18%");
  circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  circleButton->getRenderer()->setTextColor(tgui::Color::White);
  circleButton->getRenderer()->setBorders({0});
  circleButton->getRenderer()->setRoundedBorderRadius(4);
  circleButton->onPress([this, squareButton, diamondButton, circleButton, heightInput]() {
    auto brush = dynamic_pointer_cast<SpriteEraser>(project->getToolBar().getSelectedTool());
    if (brush) brush->setShape(CIRCLE);
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    heightInput->setVisible(false);
  });
  eraserSpriteOptionsPanel_->add(circleButton);
  circleButton->setTextSize(0);
}
