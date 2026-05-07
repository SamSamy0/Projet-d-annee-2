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
  auto &gui = app_.getGui();

  auto toolbar = tgui::Panel::create();
  toolbar->setSize("100%", "5%");
  toolbar->setPosition("0%", "0%");
  toolbar->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  toolbar->getRenderer()->setBorders({0});
  gui.add(toolbar, "toolbar");

  auto homeButton = tgui::Button::create();
  homeButton->setSize("2%", "70%");
  homeButton->setPosition("0.8%", "15%");
  homeButton->getRenderer()->setTexture("../res/images/accueil.png");
  homeButton->getRenderer()->setBorders({0});
  homeButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  homeButton->onPress([this, &manager]() {
    manager.getProjectList();
    app_.getNetwork().goHome();
    app_.changeView(std::make_unique<MenuView>(app_));
  });
  toolbar->add(homeButton);

  auto zoomInButton = tgui::Button::create();
  zoomInButton->setSize("2%", "70%");
  zoomInButton->setPosition("7%", "15%");
  zoomInButton->getRenderer()->setTexture("../res/images/zoom-avant.png");
  zoomInButton->getRenderer()->setBorders({0});
  zoomInButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  zoomInButton->onPress([this]() { project->getMap()->getZoom().zoomIn(); });
  toolbar->add(zoomInButton);

  auto zoomOutButton = tgui::Button::create();
  zoomOutButton->setSize("2%", "70%");
  zoomOutButton->setPosition("12%", "15%");
  zoomOutButton->getRenderer()->setTexture("../res/images/zoom-arriere.png");
  zoomOutButton->getRenderer()->setBorders({0});
  zoomOutButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  zoomOutButton->onPress([this]() { project->getMap()->getZoom().zoomOut(); });
  toolbar->add(zoomOutButton);

  if (project->getRole() != 0) {
    auto penButton = tgui::Button::create();
    auto brushButton = tgui::Button::create();
    auto shiftButton = tgui::Button::create();
    auto spriteBrushButton = tgui::Button::create();
    auto selectionButton = tgui::Button::create();
    auto fillerButton = tgui::Button::create();

    penButton->setSize("2%", "70%");
    penButton->setPosition("25%", "15%");
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

    brushButton->setSize("2%", "70%");
    brushButton->setPosition("33%", "15%");
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

    shiftButton->setSize("2%", "70%");
    shiftButton->setPosition("40.5%", "15%");
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

    spriteBrushButton->setSize("2%", "70%");
    spriteBrushButton->setPosition("48%", "15%");
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

    selectionButton->setSize("2%", "70%");
    selectionButton->setPosition("56%", "15%");
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

    fillerButton->setSize("2%", "70%");
    fillerButton->setPosition("64%", "15%");
    fillerButton->getRenderer()->setTexture("../res/images/remplissage.png");
    fillerButton->getRenderer()->setBorders({0});
    fillerButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    fillerButton->getRenderer()->setOpacity(0.4);
    fillerButton->onPress([this]() {}); // TODO : completer le bouton
    toolbar->add(fillerButton);
  }

  auto memberButton = tgui::Button::create();
  memberButton->setSize("2%", "70%");
  memberButton->setPosition("71.5%", "15%");
  memberButton->getRenderer()->setTexture("../res/images/group_users.png");
  memberButton->getRenderer()->setBorders({0});
  memberButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  memberButton->getRenderer()->setOpacity(0.4);
  memberButton->onPress(
      [this, &manager]() { manager.getUsersProjects(project->getId()); });
  toolbar->add(memberButton);

  auto exportButton = tgui::Button::create();
  exportButton->setSize("2%", "70%");
  exportButton->setPosition("79%", "15%");
  exportButton->getRenderer()->setTexture("../res/images/share.png");
  exportButton->getRenderer()->setBorders({0});
  exportButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  exportButton->getRenderer()->setOpacity(0.4);
  exportButton->onPress([this, exportButton, &manager]() {
    auto &gui = app_.getGui();
 
    //Toggle
    if (gui.get("exportPopup")) {
      exportButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
      gui.remove(gui.get("exportPopup"));
      if (gui.get("exportSubPopup"))
        gui.remove(gui.get("exportSubPopup"));
      return;
    }
    //Toggle
    if (gui.get("exportSubPopup"))
      gui.remove(gui.get("exportSubPopup"));
 
    exportButton->getRenderer()->setBackgroundColor(sf::Color(55, 55, 70));
 
    sf::Vector2f btnPos = exportButton->getAbsolutePosition();
 
    auto menu = tgui::ListBox::create();
    
    menu->getScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
    menu->getRenderer()->setTextSize(15);
    menu->getRenderer()->setBorders(2);
    menu->getRenderer()->setBorderColor(sf::Color::White);
    menu->getRenderer()->setBackgroundColor(sf::Color(40, 40, 52));
    menu->getRenderer()->setTextColor(tgui::Color::White);
    menu->addItem("Export de la carte");
    menu->addItem("Format natif");
 
    float menuHeight = menu->getItemCount() * 45;
    menu->setSize(190, menuHeight);
    menu->setItemHeight(45);
    menu->setTextSize(20);
    menu->setPosition(btnPos.x + exportButton->getSize().x - 190,
                      btnPos.y + exportButton->getSize().y);
 
    gui.add(menu, "exportPopup");
 
    menu->onItemSelect([this, menu, exportButton, &manager](const tgui::String &item) {
      auto &gui = app_.getGui();
 
      if (item == "Export de la carte") {
      //Toggle
        if (gui.get("exportSubPopup")) {
          gui.remove(gui.get("exportSubPopup"));
          menu->deselectItem();
          return;
        }
 
        sf::Vector2f menuPos = menu->getAbsolutePosition();
 
        auto subMenu = tgui::ListBox::create();
        subMenu->getScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
        subMenu->getRenderer()->setTextSize(30);
        subMenu->getRenderer()->setBorders(2);
        subMenu->getRenderer()->setBorderColor(sf::Color::White);
        subMenu->getRenderer()->setBackgroundColor(sf::Color(40, 40, 52));
        subMenu->getRenderer()->setTextColor(tgui::Color::White);
        subMenu->addItem("PNG");
        subMenu->addItem("JPG");
        subMenu->addItem("BMP");
 
        float subMenuHeight = subMenu->getItemCount() * 45;
        subMenu->setSize(120, subMenuHeight);
        subMenu->setItemHeight(45);
        subMenu->setTextSize(20);
        subMenu->setPosition(menuPos.x - 120, menuPos.y);
 
        gui.add(subMenu, "exportSubPopup");
 
        subMenu->onItemSelect([this, menu, subMenu,
                               exportButton, &manager](const tgui::String &format) {
          auto &gui = app_.getGui();
 
          std::string extension = format.toStdString();
          std::transform(extension.begin(), extension.end(), extension.begin(),
                         ::tolower);
 
          bool res = project->exportToPng(extension);
          if (res){
          popupWarning("exportPngOK");
          }else{
          popupWarning("exportPngKO");
          }
 
          exportButton->getRenderer()->setBackgroundColor(
              tgui::Color::Transparent);
          gui.remove(subMenu);
          gui.remove(menu);
        });
 
        menu->deselectItem();
 
      } else if (item == "Format natif") {
        manager.exportToNative(project->getId(), "bonjour" );
        exportButton->getRenderer()->setBackgroundColor(
            tgui::Color::Transparent);
        if (gui.get("exportSubPopup"))
          gui.remove(gui.get("exportSubPopup"));
        gui.remove(menu);
      }
    });
  });
  toolbar->add(exportButton);

  auto leaveProjectButton = tgui::Button::create();
  leaveProjectButton->setSize("2%", "70%");
  leaveProjectButton->setPosition("87%", "15%");
  leaveProjectButton->getRenderer()->setTexture("../res/images/logout.png");
  leaveProjectButton->getRenderer()->setBorders({0});
  leaveProjectButton->getRenderer()->setBackgroundColor(
      tgui::Color::Transparent);
  leaveProjectButton->getRenderer()->setOpacity(0.4);
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
