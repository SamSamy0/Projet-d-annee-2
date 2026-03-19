#include "../../../project/Tool/pixelbrush.hpp"
// #include "../../Window.hpp"
#include "../../Application.hpp"
#include "../GameView.hpp"

void GameView::initEraserOptions() {
  auto& mainWindow = app_.getWindow();
  auto& gui = app_.getGui();
  float width = mainWindow.getSize().x;
  float height = mainWindow.getSize().y;

  // Création de la barre avec toutes les infos pour la gomme
  eraserOptionsPanel_ = tgui::Panel::create();
  eraserOptionsPanel_->setSize(width * 0.35, height * 0.062);
  eraserOptionsPanel_->setPosition(width * 0.35, height * 0.05);
  eraserOptionsPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  eraserOptionsPanel_->getRenderer()->setBorders({1});
  eraserOptionsPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  eraserOptionsPanel_->getRenderer()->setRoundedBorderRadius(8);
  eraserOptionsPanel_->setVisible(false);
  gui.add(eraserOptionsPanel_);

  auto widthInput = tgui::EditBox::create();
  widthInput->setSize(width * 0.04, height * 0.04);
  widthInput->setPosition(width * 0.008, height * 0.011);
  widthInput->setDefaultText("L");
  widthInput->setInputValidator("[0-9]*");
  widthInput->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  widthInput->getRenderer()->setTextColor(tgui::Color::White);
  widthInput->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
  widthInput->getRenderer()->setBorders({1});
  widthInput->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  eraserOptionsPanel_->add(widthInput);

  auto heightInput = tgui::EditBox::create();
  heightInput->setSize(width * 0.04, height * 0.04);
  heightInput->setPosition(width * 0.052, height * 0.011);
  heightInput->setDefaultText("l");
  heightInput->setInputValidator("[0-9]*");
  heightInput->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  heightInput->getRenderer()->setTextColor(tgui::Color::White);
  heightInput->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
  heightInput->getRenderer()->setBorders({1});
  heightInput->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  heightInput->setVisible(false);
  eraserOptionsPanel_->add(heightInput);

  widthInput->onTextChange([this, widthInput, heightInput]() {
    // regarde s'il y a aucune valeur
    if (widthInput->getText().empty()) return;
    
    // Ici il regarde que la taille de la longueur (x) et largeur (y) du pinceau soit au moins égale à 1, et si y pas de valeur on prend la valeur de x
    unsigned int sizeX = static_cast<unsigned int>(std::stoi(widthInput->getText().toStdString()));
    if (sizeX < 1) sizeX = 1;
    unsigned int sizeY = heightInput->getText().empty() ? sizeX : static_cast<unsigned int>(std::stoi(heightInput->getText().toStdString()));
    if (sizeY < 1) sizeY = 1;

    // On applique la nouvelle taille
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setSize(sizeX, sizeY);
  });
  heightInput->onTextChange([this, widthInput, heightInput]() {
    // regarde s'il y a aucune valeur
    if (heightInput->getText().empty()) return;
    
    // Ici il regarde que la taille de la longueur (x) et largeur (y) du pinceau soit au moins égale à 1, et si y pas de valeur on prend la valeur de x
    unsigned int sizeX = widthInput->getText().empty() ? 1 : static_cast<unsigned int>(std::stoi(widthInput->getText().toStdString()));
    if (sizeX < 1) sizeX = 1;
    unsigned int sizeY = static_cast<unsigned int>(std::stoi(heightInput->getText().toStdString()));
    if (sizeY < 1) sizeY = 1;
    
    // On applique la nouvelle taille
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setSize(sizeX, sizeY);
  });

  // Création des différentes formes pour la gomme
  auto squareButton = tgui::Button::create("Carré");
  auto diamondButton = tgui::Button::create("Diamant");
  auto circleButton = tgui::Button::create("Cercle");

  // Carré
  squareButton->setSize(width * 0.065, height * 0.04);
  squareButton->setPosition(width * 0.1, height * 0.011);
  squareButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
  squareButton->getRenderer()->setTextColor(tgui::Color::White);
  squareButton->getRenderer()->setBorders({0});
  squareButton->getRenderer()->setRoundedBorderRadius(4);
  squareButton->onPress([this, squareButton, diamondButton, circleButton, heightInput]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setShape(SQUARE);
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    heightInput->setVisible(false);
  });
  eraserOptionsPanel_->add(squareButton);
  squareButton->setTextSize(12);

  // Diamant (losange)
  diamondButton->setSize(width * 0.065, height * 0.04);
  diamondButton->setPosition(width * 0.172, height * 0.011);
  diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  diamondButton->getRenderer()->setTextColor(tgui::Color::White);
  diamondButton->getRenderer()->setBorders({0});
  diamondButton->getRenderer()->setRoundedBorderRadius(4);
  diamondButton->onPress([this, squareButton, diamondButton, circleButton, heightInput]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setShape(DIAMOND);
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    heightInput->setVisible(true);
  });
  eraserOptionsPanel_->add(diamondButton);
  diamondButton->setTextSize(12);

  // Cercle
  circleButton->setSize(width * 0.065, height * 0.04);
  circleButton->setPosition(width * 0.244, height * 0.011);
  circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  circleButton->getRenderer()->setTextColor(tgui::Color::White);
  circleButton->getRenderer()->setBorders({0});
  circleButton->getRenderer()->setRoundedBorderRadius(4);
  circleButton->onPress([this, squareButton, diamondButton, circleButton, heightInput]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setShape(CIRCLE);
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    heightInput->setVisible(false);
  });
  eraserOptionsPanel_->add(circleButton);
  circleButton->setTextSize(12);
}
