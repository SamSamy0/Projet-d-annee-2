#include "../../Window.hpp"
#include "../../../project/Tool/spriteeraser.hpp"

void Window::initEraserSpriteOptions() {
  float width  = mainWindow.getSize().x;
  float height = mainWindow.getSize().y;

  // Création de la barre avec toutes les infos pour la gomme sprite
  eraserSpriteOptionsPanel_ = tgui::Panel::create();
  eraserSpriteOptionsPanel_->setSize(width * 0.35, height * 0.062);
  eraserSpriteOptionsPanel_->setPosition(width * 0.35, height * 0.05);
  eraserSpriteOptionsPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  eraserSpriteOptionsPanel_->getRenderer()->setBorders({1});
  eraserSpriteOptionsPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  eraserSpriteOptionsPanel_->getRenderer()->setRoundedBorderRadius(8);
  eraserSpriteOptionsPanel_->setVisible(false);
  gui.add(eraserSpriteOptionsPanel_);

  // Création de la box qui récupère la taille de la gomme
  auto sizeInputBox = tgui::EditBox::create();
  sizeInputBox->setSize(width * 0.06, height * 0.04);
  sizeInputBox->setPosition(width * 0.008, height * 0.011);
  sizeInputBox->setDefaultText("1");
  sizeInputBox->setInputValidator("[0-9]*");
  sizeInputBox->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  sizeInputBox->getRenderer()->setTextColor(tgui::Color::White);
  sizeInputBox->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
  sizeInputBox->getRenderer()->setBorders({1});
  sizeInputBox->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  sizeInputBox->onTextChange([this, sizeInputBox]() {
    if (sizeInputBox->getText().empty()) return;
    unsigned int size = static_cast<unsigned int>(std::stoi(sizeInputBox->getText().toStdString()));
    if (size < 1) {
        size = 1;
    }
    auto brush = dynamic_pointer_cast<SpriteEraser>(project->getToolBar().getSelectedTool());
    if (brush) {
        brush->setSize(size, size);
    }
  });
  eraserSpriteOptionsPanel_->add(sizeInputBox);

  // Création des différentes formes pour la gomme
  auto squareButton  = tgui::Button::create("Carré");
  auto diamondButton = tgui::Button::create("Diamant");
  auto circleButton  = tgui::Button::create("Cercle");

  // Carré
  squareButton->setSize(width * 0.065, height * 0.04);
  squareButton->setPosition(width * 0.1, height * 0.011);
  squareButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
  squareButton->getRenderer()->setTextColor(tgui::Color::White);
  squareButton->getRenderer()->setBorders({0});
  squareButton->getRenderer()->setRoundedBorderRadius(4);
  squareButton->onPress([this, squareButton, diamondButton, circleButton]() {
    auto brush = dynamic_pointer_cast<SpriteEraser>(project->getToolBar().getSelectedTool());
    if (brush) {
        brush->setShape(SQUARE);
    }
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  });
  eraserSpriteOptionsPanel_->add(squareButton);
  squareButton->setTextSize(12);

  // Diamant (losange)
  diamondButton->setSize(width * 0.065, height * 0.04);
  diamondButton->setPosition(width * 0.172, height * 0.011);
  diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  diamondButton->getRenderer()->setTextColor(tgui::Color::White);
  diamondButton->getRenderer()->setBorders({0});
  diamondButton->getRenderer()->setRoundedBorderRadius(4);
  diamondButton->onPress([this, squareButton, diamondButton, circleButton]() {
    auto brush = dynamic_pointer_cast<SpriteEraser>(project->getToolBar().getSelectedTool());
    if (brush) {
        brush->setShape(DIAMOND);
    }
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  });
  eraserSpriteOptionsPanel_->add(diamondButton);
  diamondButton->setTextSize(12);

  // Cercle
  circleButton->setSize(width * 0.065, height * 0.04);
  circleButton->setPosition(width * 0.244, height * 0.011);
  circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  circleButton->getRenderer()->setTextColor(tgui::Color::White);
  circleButton->getRenderer()->setBorders({0});
  circleButton->getRenderer()->setRoundedBorderRadius(4);
  circleButton->onPress([this, squareButton, diamondButton, circleButton]() {
    auto brush = dynamic_pointer_cast<SpriteEraser>(project->getToolBar().getSelectedTool());
    if (brush) {
        brush->setShape(CIRCLE);
    }
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
  });
  eraserSpriteOptionsPanel_->add(circleButton);
  circleButton->setTextSize(12);
}
