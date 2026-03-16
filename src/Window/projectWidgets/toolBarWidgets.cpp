#include "../Window.hpp"
#include "../../project/Tool/pixelbrush.hpp"
#include "../../project/Tool/pixelshift.hpp"
#include "../../project/Layer/layer.hpp"

void Window::initToolbar() {
  float width  = mainWindow.getSize().x;
  float height = mainWindow.getSize().y;

  // Création du panel (box) qui reprends tout les outils
  auto toolbar = tgui::Panel::create();
  toolbar->setSize(width, height * 0.05);
  toolbar->setPosition(width * 0.0, height * 0.0);
  toolbar->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  toolbar->getRenderer()->setBorders({0});
  gui.add(toolbar, "toolbar");

  // Création du bouton home (retour au menu)
  auto homeButton = tgui::Button::create();
  homeButton->setSize(height * 0.035, height * 0.035);
  homeButton->setPosition(width * 0.008, height * 0.007);
  homeButton->getRenderer()->setTexture("../res/images/accueil.png");
  homeButton->getRenderer()->setBorders({0});
  homeButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  homeButton->onPress(&Window::setState, this, projectState::MENU);
  toolbar->add(homeButton);

  // Création du bouton de zoom avant
  auto zoomInButton = tgui::Button::create();
  zoomInButton->setSize(height * 0.035, height * 0.035);
  zoomInButton->setPosition(width * 0.07, height * 0.007);
  zoomInButton->getRenderer()->setTexture("../res/images/zoom-avant.png");
  zoomInButton->getRenderer()->setBorders({0});
  zoomInButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  zoomInButton->onPress([this]() { project->getMap()->getZoom().zoomIn(); });
  toolbar->add(zoomInButton);

  // Création du bouton de zoom arrière
  auto zoomOutButton = tgui::Button::create();
  zoomOutButton->setSize(height * 0.035, height * 0.035);
  zoomOutButton->setPosition(width * 0.12, height * 0.007);
  zoomOutButton->getRenderer()->setTexture("../res/images/zoom-arriere.png");
  zoomOutButton->getRenderer()->setBorders({0});
  zoomOutButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  zoomOutButton->onPress([this]() { project->getMap()->getZoom().zoomOut(); });
  toolbar->add(zoomOutButton);

  // Création des boutons pour les outils pixels
  auto penButton   = tgui::Button::create();
  auto brushButton = tgui::Button::create();
  auto shiftButton = tgui::Button::create();
  auto spriteBrushButton = tgui::Button::create();

  // Options pour le crayon
  penButton->setSize(height * 0.035, height * 0.035);
  penButton->setPosition(width * 0.25, height * 0.007);
  penButton->getRenderer()->setTexture("../res/images/crayon.png");
  penButton->getRenderer()->setBorders({0});
  penButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  penButton->getRenderer()->setOpacity(0.4);
  penButton->onPress([this, penButton, brushButton, shiftButton, spriteBrushButton]() {
    LayerType layerType = project->getMap()->getCurrentLayer()->getType();
    if (layerType == SPRITELAYER) {
      project->getToolBar().selectTool(SPRITEBRUSH);
    } else {
      project->getToolBar().selectTool(PIXELBRUSH);
      dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool())->setEraser(false);
    }
    penButton->getRenderer()->setOpacity(1.0);
    brushButton->getRenderer()->setOpacity(0.4);
    shiftButton->getRenderer()->setOpacity(0.4);
    spriteBrushButton->getRenderer()->setOpacity(0.4);
    if (penOptionsPanel_) {
        penOptionsPanel_->setVisible(!penOptionsPanel_->isVisible());
    }
    if (eraserOptionsPanel_) {
        eraserOptionsPanel_->setVisible(false);
    }
    if (spriteBrushOptionsPanel_) {
        spriteBrushOptionsPanel_->setVisible(false);
    }
  });
  toolbar->add(penButton);

  // Options pour la gomme
  brushButton->setSize(height * 0.035, height * 0.035);
  brushButton->setPosition(width * 0.35, height * 0.007);
  brushButton->getRenderer()->setTexture("../res/images/la-gomme.png");
  brushButton->getRenderer()->setBorders({0});
  brushButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  brushButton->getRenderer()->setOpacity(0.4);
  brushButton->onPress([this, penButton, brushButton, shiftButton, spriteBrushButton]() {
    LayerType layerType = project->getMap()->getCurrentLayer()->getType();
    if (layerType == SPRITELAYER) {
      project->getToolBar().selectTool(SPRITEERASER);
    } else {
      project->getToolBar().selectTool(PIXELBRUSH);
      dynamic_pointer_cast<PixelBrush>(project->getToolBar().getSelectedTool())->setEraser(true);
    }
    penButton->getRenderer()->setOpacity(0.4);
    brushButton->getRenderer()->setOpacity(1.0);
    shiftButton->getRenderer()->setOpacity(0.4);
    spriteBrushButton->getRenderer()->setOpacity(0.4);
    if (penOptionsPanel_) {
        penOptionsPanel_->setVisible(false);
    }
    if (eraserOptionsPanel_) {
        eraserOptionsPanel_->setVisible(!eraserOptionsPanel_->isVisible());
    }
    if (spriteBrushOptionsPanel_) {
        spriteBrushOptionsPanel_->setVisible(false);
    }
  });
  toolbar->add(brushButton);

  // Options pour le shift pixel
  shiftButton->setSize(height * 0.035, height * 0.035);
  shiftButton->setPosition(width * 0.45, height * 0.007);
  shiftButton->getRenderer()->setTexture("../res/images/couche.png");
  shiftButton->getRenderer()->setBorders({0});
  shiftButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  shiftButton->getRenderer()->setOpacity(0.4);
  shiftButton->onPress([this, penButton, brushButton, shiftButton, spriteBrushButton]() {
    LayerType layerType = project->getMap()->getCurrentLayer()->getType();
    if (layerType == SPRITELAYER) {
      project->getToolBar().selectTool(SPRITESHIFT);
    } else {
      project->getToolBar().selectTool(PIXELSHIFT);
    }
    penButton->getRenderer()->setOpacity(0.4);
    brushButton->getRenderer()->setOpacity(0.4);
    shiftButton->getRenderer()->setOpacity(1.0);
    spriteBrushButton->getRenderer()->setOpacity(0.4);
    if (penOptionsPanel_) {
        penOptionsPanel_->setVisible(false);
    }
    if (eraserOptionsPanel_) {
        eraserOptionsPanel_->setVisible(false);
    }
    if (spriteBrushOptionsPanel_) {
        spriteBrushOptionsPanel_->setVisible(false);
    }
  });
  toolbar->add(shiftButton);

  // Options pour le bouton sprite brush
  spriteBrushButton->setSize(height * 0.035, height * 0.035);
  spriteBrushButton->setPosition(width * 0.55, height * 0.007);
  spriteBrushButton->getRenderer()->setTexture("../res/images/sprite-brush.png");
  spriteBrushButton->getRenderer()->setBorders({0});
  spriteBrushButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  spriteBrushButton->getRenderer()->setOpacity(0.4);
  spriteBrushButton->onPress([this, penButton, brushButton, shiftButton, spriteBrushButton]() {
    project->getToolBar().selectTool(SPRITEBRUSH);
    penButton->getRenderer()->setOpacity(0.4);
    brushButton->getRenderer()->setOpacity(0.4);
    shiftButton->getRenderer()->setOpacity(0.4);
    spriteBrushButton->getRenderer()->setOpacity(1.0);
    if (penOptionsPanel_) {
        penOptionsPanel_->setVisible(false);
    }
    if (eraserOptionsPanel_) {
        eraserOptionsPanel_->setVisible(false);
    }
    if (spriteBrushOptionsPanel_) {
        spriteBrushOptionsPanel_->setVisible(true);
    }
  });
  toolbar->add(spriteBrushButton);
}
