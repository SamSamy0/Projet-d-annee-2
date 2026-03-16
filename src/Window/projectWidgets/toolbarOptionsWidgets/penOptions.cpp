#include "../../Window.hpp"
#include "../../../project/Tool/pixelbrush.hpp"

void Window::initPenOptions() {
  float width  = mainWindow.getSize().x;
  float height = mainWindow.getSize().y;

  // Création de la barre avec tous les options pour l'outil crayon
  penOptionsPanel_ = tgui::Panel::create();
  penOptionsPanel_->setSize(width * 0.76, height * 0.062);
  penOptionsPanel_->setPosition(width * 0.22, height * 0.05);
  penOptionsPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  penOptionsPanel_->getRenderer()->setBorders({1});
  penOptionsPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  penOptionsPanel_->getRenderer()->setRoundedBorderRadius(8);
  penOptionsPanel_->setVisible(false);
  gui.add(penOptionsPanel_);

  // Création de la box qui récupère la longueur
  auto lengthInput = tgui::EditBox::create();
  lengthInput->setSize(width * 0.04, height * 0.04);
  lengthInput->setPosition(width * 0.008, height * 0.011);
  lengthInput->setDefaultText("L");
  lengthInput->setInputValidator("[0-9]*\\.?[0-9]*");
  lengthInput->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  lengthInput->getRenderer()->setTextColor(tgui::Color::White);
  lengthInput->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
  lengthInput->getRenderer()->setBorders({1});
  lengthInput->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  penOptionsPanel_->add(lengthInput);

  // Création de la box qui récupère la largeur
  auto widthInput = tgui::EditBox::create();
  widthInput->setSize(width * 0.04, height * 0.04);
  widthInput->setPosition(width * 0.052, height * 0.011);
  widthInput->setDefaultText("W");
  widthInput->setInputValidator("[0-9]*\\.?[0-9]*");
  widthInput->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  widthInput->getRenderer()->setTextColor(tgui::Color::White);
  widthInput->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
  widthInput->getRenderer()->setBorders({1});
  widthInput->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  widthInput->setVisible(false);
  penOptionsPanel_->add(widthInput);

  lengthInput->onTextChange([this, lengthInput, widthInput]() {
    if (lengthInput->getText().empty()) return;

    float sizeX = lengthInput->getText().empty() ? 1 :std::stof(lengthInput->getText().toStdString());

    float sizeY = widthInput->getText().empty() ? 1 : std::stof(widthInput->getText().toStdString());

    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) {
      brush->setSize(sizeX, sizeY);
    }
  });
  widthInput->onTextChange([this, lengthInput, widthInput]() {
    if (widthInput->getText().empty()) return;
    float sizeX = lengthInput->getText().empty() ? 1 : std::stof(lengthInput->getText().toStdString());
    float sizeY =widthInput->getText().empty() ? 1 : std::stoi(widthInput->getText().toStdString());
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) {
      brush->setSize(sizeX, sizeY);
    }
  });

  // Création des différentes formes pour le crayon
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
  squareButton->onPress([this, squareButton, diamondButton, circleButton, widthInput]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) {
        brush->setShape(SQUARE);
    }
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    widthInput->setVisible(false);
  });
  penOptionsPanel_->add(squareButton);
  squareButton->setTextSize(12);

  // Diamant
  diamondButton->setSize(width * 0.065, height * 0.04);
  diamondButton->setPosition(width * 0.172, height * 0.011);
  diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  diamondButton->getRenderer()->setTextColor(tgui::Color::White);
  diamondButton->getRenderer()->setBorders({0});
  diamondButton->getRenderer()->setRoundedBorderRadius(4);
  diamondButton->onPress([this, squareButton, diamondButton, circleButton, widthInput]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) {
        brush->setShape(DIAMOND);
    }
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    widthInput->setVisible(true);
  });
  penOptionsPanel_->add(diamondButton);
  diamondButton->setTextSize(12);

  // Cercle
  circleButton->setSize(width * 0.065, height * 0.04);
  circleButton->setPosition(width * 0.244, height * 0.011);
  circleButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  circleButton->getRenderer()->setTextColor(tgui::Color::White);
  circleButton->getRenderer()->setBorders({0});
  circleButton->getRenderer()->setRoundedBorderRadius(4);
  circleButton->onPress([this, squareButton, diamondButton, circleButton, widthInput]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) {
        brush->setShape(CIRCLE);
    }
    squareButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    diamondButton->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
    circleButton->getRenderer()->setBackgroundColor(tgui::Color(60, 130, 200));
    widthInput->setVisible(false);
  });
  penOptionsPanel_->add(circleButton);
  circleButton->setTextSize(12);

  // Création des couleurs prédéfinies
  auto blackButton  = tgui::Button::create();
  auto whiteButton  = tgui::Button::create();
  auto greenButton  = tgui::Button::create();
  auto blueButton   = tgui::Button::create();
  auto yellowButton = tgui::Button::create();

  // Noir
  blackButton->setSize(width * 0.033, height * 0.04);
  blackButton->setPosition(width * 0.325, height * 0.011);
  blackButton->getRenderer()->setBackgroundColor(tgui::Color(0, 0, 0));
  blackButton->getRenderer()->setBackgroundColorHover(tgui::Color(0, 0, 0));
  blackButton->getRenderer()->setBorders({2});
  blackButton->getRenderer()->setBorderColor(tgui::Color(200, 220, 255));
  blackButton->getRenderer()->setRoundedBorderRadius(3);
  blackButton->onPress([this, blackButton, whiteButton, greenButton, blueButton, yellowButton]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) {
        brush->setColor(sf::Color::Black);
    }
    blackButton->getRenderer()->setBorders({2});
    blackButton->getRenderer()->setBorderColor(tgui::Color(200, 220, 255));
    whiteButton->getRenderer()->setBorders({0});
    greenButton->getRenderer()->setBorders({0});
    blueButton->getRenderer()->setBorders({0});
    yellowButton->getRenderer()->setBorders({0});
  });
  penOptionsPanel_->add(blackButton);

  // Blanc
  whiteButton->setSize(width * 0.033, height * 0.04);
  whiteButton->setPosition(width * 0.363, height * 0.011);
  whiteButton->getRenderer()->setBackgroundColor(tgui::Color(255, 255, 255));
  whiteButton->getRenderer()->setBackgroundColorHover(tgui::Color(255, 255, 255));
  whiteButton->getRenderer()->setBorders({0});
  whiteButton->getRenderer()->setRoundedBorderRadius(3);
  whiteButton->onPress([this, blackButton, whiteButton, greenButton, blueButton, yellowButton]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) {
        brush->setColor(sf::Color::White);
    }
    blackButton->getRenderer()->setBorders({0});
    whiteButton->getRenderer()->setBorders({2});
    whiteButton->getRenderer()->setBorderColor(tgui::Color(200, 220, 255));
    greenButton->getRenderer()->setBorders({0});
    blueButton->getRenderer()->setBorders({0});
    yellowButton->getRenderer()->setBorders({0});
  });
  penOptionsPanel_->add(whiteButton);

  // Vert
  greenButton->setSize(width * 0.033, height * 0.04);
  greenButton->setPosition(width * 0.401, height * 0.011);
  greenButton->getRenderer()->setBackgroundColor(tgui::Color(50, 200, 80));
  greenButton->getRenderer()->setBackgroundColorHover(tgui::Color(50, 200, 80));
  greenButton->getRenderer()->setBorders({0});
  greenButton->getRenderer()->setRoundedBorderRadius(3);
  greenButton->onPress([this, blackButton, whiteButton, greenButton, blueButton, yellowButton]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) {
        brush->setColor(sf::Color(50, 200, 80));
    }
    blackButton->getRenderer()->setBorders({0});
    whiteButton->getRenderer()->setBorders({0});
    greenButton->getRenderer()->setBorders({2});
    greenButton->getRenderer()->setBorderColor(tgui::Color(200, 220, 255));
    blueButton->getRenderer()->setBorders({0});
    yellowButton->getRenderer()->setBorders({0});
  });
  penOptionsPanel_->add(greenButton);

  // Bleu
  blueButton->setSize(width * 0.033, height * 0.04);
  blueButton->setPosition(width * 0.439, height * 0.011);
  blueButton->getRenderer()->setBackgroundColor(tgui::Color(30, 100, 255));
  blueButton->getRenderer()->setBackgroundColorHover(tgui::Color(30, 100, 255));
  blueButton->getRenderer()->setBorders({0});
  blueButton->getRenderer()->setRoundedBorderRadius(3);
  blueButton->onPress([this, blackButton, whiteButton, greenButton, blueButton, yellowButton]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) {
        brush->setColor(sf::Color(30, 100, 255));
    }
    blackButton->getRenderer()->setBorders({0});
    whiteButton->getRenderer()->setBorders({0});
    greenButton->getRenderer()->setBorders({0});
    blueButton->getRenderer()->setBorders({2});
    blueButton->getRenderer()->setBorderColor(tgui::Color(200, 220, 255));
    yellowButton->getRenderer()->setBorders({0});
  });
  penOptionsPanel_->add(blueButton);

  // Yellow
  yellowButton->setSize(width * 0.033, height * 0.04);
  yellowButton->setPosition(width * 0.477, height * 0.011);
  yellowButton->getRenderer()->setBackgroundColor(tgui::Color(255, 220, 0));
  yellowButton->getRenderer()->setBackgroundColorHover(tgui::Color(255, 220, 0));
  yellowButton->getRenderer()->setBorders({0});
  yellowButton->getRenderer()->setRoundedBorderRadius(3);
  yellowButton->onPress([this, blackButton, whiteButton, greenButton, blueButton, yellowButton]() {
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) {
        brush->setColor(sf::Color(255, 220, 0));
    }
    blackButton->getRenderer()->setBorders({0});
    whiteButton->getRenderer()->setBorders({0});
    greenButton->getRenderer()->setBorders({0});
    blueButton->getRenderer()->setBorders({0});
    yellowButton->getRenderer()->setBorders({2});
    yellowButton->getRenderer()->setBorderColor(tgui::Color(200, 220, 255));
  });
  penOptionsPanel_->add(yellowButton);

  // Je crée ici la possibilité de faire une couleur personnalisé
  auto rInput = tgui::EditBox::create();
  rInput->setSize(width * 0.038, height * 0.04);
  rInput->setPosition(width * 0.522, height * 0.011);
  rInput->setDefaultText("R");
  rInput->setInputValidator("[0-9]*");
  rInput->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  rInput->getRenderer()->setTextColor(tgui::Color::White);
  rInput->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
  rInput->getRenderer()->setBorders({1});
  rInput->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  penOptionsPanel_->add(rInput);

  auto gInput = tgui::EditBox::create();
  gInput->setSize(width * 0.038, height * 0.04);
  gInput->setPosition(width * 0.564, height * 0.011);
  gInput->setDefaultText("G");
  gInput->setInputValidator("[0-9]*");
  gInput->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  gInput->getRenderer()->setTextColor(tgui::Color::White);
  gInput->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
  gInput->getRenderer()->setBorders({1});
  gInput->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  penOptionsPanel_->add(gInput);

  auto bInput = tgui::EditBox::create();
  bInput->setSize(width * 0.038, height * 0.04);
  bInput->setPosition(width * 0.606, height * 0.011);
  bInput->setDefaultText("B");
  bInput->setInputValidator("[0-9]*");
  bInput->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  bInput->getRenderer()->setTextColor(tgui::Color::White);
  bInput->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
  bInput->getRenderer()->setBorders({1});
  bInput->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  penOptionsPanel_->add(bInput);

  auto aInput = tgui::EditBox::create();
  aInput->setSize(width * 0.038, height * 0.04);
  aInput->setPosition(width * 0.648, height * 0.011);
  aInput->setDefaultText("A");
  aInput->setInputValidator("[0-9]*");
  aInput->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  aInput->getRenderer()->setTextColor(tgui::Color::White);
  aInput->getRenderer()->setDefaultTextColor(tgui::Color(150, 155, 165));
  aInput->getRenderer()->setBorders({1});
  aInput->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  penOptionsPanel_->add(aInput);

  auto applyRGBButton = tgui::Button::create("OK");
  applyRGBButton->setSize(width * 0.033, height * 0.04);
  applyRGBButton->setPosition(width * 0.690, height * 0.011);
  applyRGBButton->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
  applyRGBButton->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
  applyRGBButton->getRenderer()->setTextColor(tgui::Color::White);
  applyRGBButton->getRenderer()->setBorders({0});
  applyRGBButton->getRenderer()->setRoundedBorderRadius(4);
  applyRGBButton->onPress([this, rInput, gInput, bInput, aInput]() {
    int r = rInput->getText().empty() ? 0   : std::min(255, std::stoi(rInput->getText().toStdString()));
    int g = gInput->getText().empty() ? 0   : std::min(255, std::stoi(gInput->getText().toStdString()));
    int b = bInput->getText().empty() ? 0   : std::min(255, std::stoi(bInput->getText().toStdString()));
    int a = aInput->getText().empty() ? 255 : std::min(255, std::stoi(aInput->getText().toStdString()));
    auto brush = dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool());
    if (brush) brush->setColor(sf::Color(r, g, b, a));
  });
  penOptionsPanel_->add(applyRGBButton);
}
