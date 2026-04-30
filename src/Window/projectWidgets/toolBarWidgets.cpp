// #include "../Window.hpp"
#include "../../project/Layer/layer.hpp"
#include "../../project/Tool/pixelbrush.hpp"
#include "../../project/Tool/pixelshift.hpp"
#include "../Application.hpp"
#include "../MenuView.hpp"
#include "GameView.hpp"
#include <memory>

void GameView::initToolbar() {
  auto &manager = app_.getNetwork();
  auto &mainWindow = app_.getWindow();
  auto &gui = app_.getGui();
  float width = mainWindow.getSize().x;
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
  homeButton->onPress([this, &manager]() {
    manager.getProjectList();
    app_.changeView(std::make_unique<MenuView>(app_));
  });
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

  if (project->getRole() != 0) {
    // Création des boutons pour les outils pixels
    auto penButton = tgui::Button::create();
    auto brushButton = tgui::Button::create();
    auto shiftButton = tgui::Button::create();
    auto spriteBrushButton = tgui::Button::create();
    auto selectionButton = tgui::Button::create();

    // Options pour le crayon
    penButton->setSize(height * 0.035, height * 0.035);
    penButton->setPosition(width * 0.25, height * 0.007);
    penButton->getRenderer()->setTexture("../res/images/crayon.png");
    penButton->getRenderer()->setBorders({0});
    penButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    penButton->getRenderer()->setOpacity(0.4);
    penButton->onPress([this, penButton, brushButton, shiftButton,
                        spriteBrushButton, selectionButton]() {
      LayerType layerType = project->getMap()->getCurrentLayer()->getType();
      if (layerType == SPRITELAYER) {
        project->getToolBar().selectTool(SPRITEBRUSH);
      } else {
        project->getToolBar().selectTool(PIXELBRUSH);
        dynamic_pointer_cast<PixelBrush>(
            project->getToolBar().getSelectedTool())
            ->setEraser(false);
      }
      penButton->getRenderer()->setOpacity(1.0);
      brushButton->getRenderer()->setOpacity(0.4);
      shiftButton->getRenderer()->setOpacity(0.4);
      spriteBrushButton->getRenderer()->setOpacity(0.4);
      selectionButton->getRenderer()->setOpacity(0.4);
      if (layerType == SPRITELAYER) {
        if (penOptionsPanel_)
          penOptionsPanel_->setVisible(false);
        if (penSpriteOptionsPanel_)
          penSpriteOptionsPanel_->setVisible(
              !penSpriteOptionsPanel_->isVisible());
        if (eraserOptionsPanel_)
          eraserOptionsPanel_->setVisible(false);
        if (eraserSpriteOptionsPanel_)
          eraserSpriteOptionsPanel_->setVisible(false);
        if (spriteBrushOptionsPanel_)
          spriteBrushOptionsPanel_->setVisible(false);
      } else {
        if (penOptionsPanel_)
          penOptionsPanel_->setVisible(!penOptionsPanel_->isVisible());
        if (penSpriteOptionsPanel_)
          penSpriteOptionsPanel_->setVisible(false);
        if (eraserOptionsPanel_)
          eraserOptionsPanel_->setVisible(false);
        if (eraserSpriteOptionsPanel_)
          eraserSpriteOptionsPanel_->setVisible(false);
        if (spriteBrushOptionsPanel_)
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
    brushButton->onPress([this, penButton, brushButton, shiftButton,
                          spriteBrushButton, selectionButton]() {
      LayerType layerType = project->getMap()->getCurrentLayer()->getType();
      if (layerType == SPRITELAYER) {
        project->getToolBar().selectTool(SPRITEERASER);
      } else {
        project->getToolBar().selectTool(PIXELBRUSH);
        dynamic_pointer_cast<PixelBrush>(
            project->getToolBar().getSelectedTool())
            ->setEraser(true);
      }
      penButton->getRenderer()->setOpacity(0.4);
      brushButton->getRenderer()->setOpacity(1.0);
      shiftButton->getRenderer()->setOpacity(0.4);
      spriteBrushButton->getRenderer()->setOpacity(0.4);
      selectionButton->getRenderer()->setOpacity(0.4);
      if (penOptionsPanel_)
        penOptionsPanel_->setVisible(false);
      if (penSpriteOptionsPanel_)
        penSpriteOptionsPanel_->setVisible(false);
      if (spriteBrushOptionsPanel_)
        spriteBrushOptionsPanel_->setVisible(false);
      if (layerType == SPRITELAYER) {
        if (eraserOptionsPanel_)
          eraserOptionsPanel_->setVisible(false);
        if (eraserSpriteOptionsPanel_) {
          if (eraserSpriteOptionsPanel_->isVisible())
            eraserSpriteOptionsPanel_->setVisible(false);
          else
            eraserSpriteOptionsPanel_->setVisible(true);
        }
      } else {
        if (eraserSpriteOptionsPanel_)
          eraserSpriteOptionsPanel_->setVisible(false);
        if (eraserOptionsPanel_) {
          if (eraserOptionsPanel_->isVisible())
            eraserOptionsPanel_->setVisible(false);
          else
            eraserOptionsPanel_->setVisible(true);
        }
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
    shiftButton->onPress([this, penButton, brushButton, shiftButton,
                          spriteBrushButton, selectionButton]() {
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
      selectionButton->getRenderer()->setOpacity(0.4);
      if (penOptionsPanel_)
        penOptionsPanel_->setVisible(false);
      if (penSpriteOptionsPanel_)
        penSpriteOptionsPanel_->setVisible(false);
      if (eraserOptionsPanel_)
        eraserOptionsPanel_->setVisible(false);
      if (eraserSpriteOptionsPanel_)
        eraserSpriteOptionsPanel_->setVisible(false);
      if (spriteBrushOptionsPanel_)
        spriteBrushOptionsPanel_->setVisible(false);
    });
    toolbar->add(shiftButton);

    // Options pour le bouton sprite brush
    spriteBrushButton->setSize(height * 0.035, height * 0.035);
    spriteBrushButton->setPosition(width * 0.55, height * 0.007);
    spriteBrushButton->getRenderer()->setTexture(
        "../res/images/sprite-brush.png");
    spriteBrushButton->getRenderer()->setBorders({0});
    spriteBrushButton->getRenderer()->setBackgroundColor(
        tgui::Color::Transparent);
    spriteBrushButton->getRenderer()->setOpacity(0.4);
    spriteBrushButton->onPress([this, penButton, brushButton, shiftButton,
                                spriteBrushButton, selectionButton]() {
      project->getToolBar().selectTool(SPRITEBRUSH);
      penButton->getRenderer()->setOpacity(0.4);
      brushButton->getRenderer()->setOpacity(0.4);
      shiftButton->getRenderer()->setOpacity(0.4);
      spriteBrushButton->getRenderer()->setOpacity(1.0);
      selectionButton->getRenderer()->setOpacity(0.4);
      if (penOptionsPanel_)
        penOptionsPanel_->setVisible(false);
      if (penSpriteOptionsPanel_)
        penSpriteOptionsPanel_->setVisible(false);
      if (eraserOptionsPanel_)
        eraserOptionsPanel_->setVisible(false);
      if (eraserSpriteOptionsPanel_)
        eraserSpriteOptionsPanel_->setVisible(false);
      if (spriteBrushOptionsPanel_)
        spriteBrushOptionsPanel_->setVisible(
            !spriteBrushOptionsPanel_->isVisible());
    });
    toolbar->add(spriteBrushButton);

    // Options pour le bouton de sélection
    selectionButton->setSize(height * 0.035, height * 0.035);
    selectionButton->setPosition(width * 0.65, height * 0.007);
    selectionButton->getRenderer()->setTexture("../res/images/selection.png");
    selectionButton->getRenderer()->setBorders({0});
    selectionButton->getRenderer()->setBackgroundColor(
        tgui::Color::Transparent);
    selectionButton->getRenderer()->setOpacity(0.4);
    selectionButton->onPress([this, penButton, brushButton, shiftButton,
                              spriteBrushButton, selectionButton]() {
      project->getToolBar().selectTool(SPRITESELECTION);
      penButton->getRenderer()->setOpacity(0.4);
      brushButton->getRenderer()->setOpacity(0.4);
      shiftButton->getRenderer()->setOpacity(0.4);
      spriteBrushButton->getRenderer()->setOpacity(0.4);
      selectionButton->getRenderer()->setOpacity(1.0);
      if (penOptionsPanel_)
        penOptionsPanel_->setVisible(false);
      if (penSpriteOptionsPanel_)
        penSpriteOptionsPanel_->setVisible(false);
      if (eraserOptionsPanel_)
        eraserOptionsPanel_->setVisible(false);
      if (eraserSpriteOptionsPanel_)
        eraserSpriteOptionsPanel_->setVisible(false);
      if (spriteBrushOptionsPanel_)
        spriteBrushOptionsPanel_->setVisible(false);
    });
    toolbar->add(selectionButton);
  }

  auto memberButton = tgui::Button::create();
  memberButton->setSize(height * 0.035, height * 0.040);
  memberButton->setPosition(width * 0.75, height * 0.007);
  memberButton->getRenderer()->setTexture("../res/images/group_users.png");
  memberButton->getRenderer()->setBorders({0});
  memberButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  memberButton->getRenderer()->setOpacity(0.4);
  memberButton->getRenderer()->setTextureHover("../res/images/group_users.png");
  memberButton->onPress(
      [this, &manager]() { manager.getUsersProjects(project->getId()); });
  toolbar->add(memberButton);

  //Made by IA for better Design 
  //(based on a dropdown menu that I've already made (showProjectMenu in MenuView))
  auto exportButton = tgui::Button::create();
  exportButton->setSize(height * 0.035, height * 0.040);
  exportButton->setPosition(width * 0.85, height * 0.007);
  exportButton->getRenderer()->setTexture("../res/images/share.png");
  exportButton->getRenderer()->setBorders({0});
  exportButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  exportButton->getRenderer()->setOpacity(0.4);
  exportButton->getRenderer()->setTextureHover("../res/images/share.png");
  exportButton->onPress([this,&manager, exportButton, height]() {
    auto &gui = app_.getGui();
 
    //Toggle
    if (gui.get("exportPopup")) {
      exportButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
      gui.remove(gui.get("exportPopup"));
      return;
    }
 
    exportButton->getRenderer()->setBackgroundColor(sf::Color(55, 55, 70));
 
    sf::Vector2f btnPos = exportButton->getAbsolutePosition();
 
    auto menu = tgui::ListBox::create();
    menu->getScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
    menu->getRenderer()->setTextSize(20);
    menu->getRenderer()->setBorders(2);
    menu->getRenderer()->setBorderColor(sf::Color::White);
    menu->getRenderer()->setBackgroundColor(sf::Color(40, 40, 52));
    menu->getRenderer()->setTextColor(tgui::Color::White);
    menu->addItem("Exporter en PNG");
    menu->addItem("Format natif");
 
    float menuHeight = menu->getItemCount() * 45;
    menu->setSize(180, menuHeight);
    menu->setItemHeight(45);
    menu->setTextSize(20);
    menu->setPosition(btnPos.x + exportButton->getSize().x - 180,
                      btnPos.y + exportButton->getSize().y);
 
    gui.add(menu, "exportPopup");
 
    menu->onItemSelect([this, &manager, menu, exportButton](const tgui::String &item) {
      auto &gui = app_.getGui();
 
      if (item == "Exporter en PNG") {
        project->exportToPng();
      } else if (item == "Format natif") {
        manager.exportToNative(project->getId());
      }
 
      exportButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
      gui.remove(menu);
    });
  });
  toolbar->add(exportButton);

  auto leaveProjectButton = tgui::Button::create();
  leaveProjectButton->setSize(height * 0.035, height * 0.040);
  leaveProjectButton->setPosition(width * 0.925, height * 0.007);
  leaveProjectButton->getRenderer()->setTexture("../res/images/logout.png");
  leaveProjectButton->getRenderer()->setBorders({0});
  leaveProjectButton->getRenderer()->setBackgroundColor(
      tgui::Color::Transparent);
  leaveProjectButton->getRenderer()->setOpacity(0.4);
  leaveProjectButton->getRenderer()->setTextureHover(
      "../res/images/logout.png");
  leaveProjectButton->onPress([this, &manager]() {
    if (project->getRole() == 2) {
      Transferring = true;
      manager.getUsersProjects(project->getId());
    } else {
      manager.delProject(project->getId());
      manager.getProjectList();
      app_.changeView(std::make_unique<MenuView>(app_));
    }
  });
  toolbar->add(leaveProjectButton);
}
