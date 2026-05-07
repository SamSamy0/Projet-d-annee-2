#include "MenuView.hpp"
#include "Application.hpp"
#include "View.hpp"
#include "projectWidgets/GameView.hpp"
#include <QFile>
#include <portable-file-dialogs.h>

MenuView::MenuView(Application &app) : View(app) {};

void MenuView::init() {
  auto &gui = app_.getGui();
  gui.removeAllWidgets();
  // app_.getNetwork().getProjectList();

  // Left Background
  auto leftPanel = tgui::Panel::create();
  leftPanel->setSize("55%", "100%");
  leftPanel->getRenderer()->setBackgroundColor(sf::Color(22, 22, 28));
  gui.add(leftPanel);

  // Title
  auto title = tgui::Label::create("Mes Projets");
  title->setPosition("5%", "6%");
  title->setTextSize(28);
  title->getRenderer()->setTextColor(sf::Color(255, 255, 255));
  leftPanel->add(title);

  // Projects Counter
  auto counter =
      tgui::Label::create(std::to_string(projectList.size()) + " projets");
  counter->setPosition("5%", "11%");
  counter->setTextSize(13);
  counter->getRenderer()->setTextColor(sf::Color(120, 120, 140));
  leftPanel->add(counter);

  auto sep = tgui::Panel::create();
  sep->setSize("90%", 1);
  sep->setPosition("5%", "16%");
  sep->getRenderer()->setBackgroundColor(sf::Color(45, 45, 55));
  leftPanel->add(sep);

  displayProjList(leftPanel);

  auto rightPanel = tgui::Panel::create();
  rightPanel->setSize("45%", "100%");
  rightPanel->setPosition("55%", 0);
  rightPanel->getRenderer()->setBackgroundColor(sf::Color(28, 28, 35));
  gui.add(rightPanel);

  auto createProjB = tgui::Button::create("+ Créer un projet");
  createProjB->setPosition("10%", "10%");
  createProjB->setSize("80%", "6%");
  createProjB->getRenderer()->setBackgroundColor(sf::Color(99, 102, 241));
  createProjB->getRenderer()->setBackgroundColorHover(sf::Color(118, 120, 255));
  createProjB->getRenderer()->setTextColor(sf::Color::White);
  createProjB->getRenderer()->setBorders(0);
  createProjB->getRenderer()->setRoundedBorderRadius(8);
  rightPanel->add(createProjB);
  createProjB->onPress(&MenuView::askProjectData, this);

  auto joinProjB = tgui::Button::create("Rejoindre un projet");
  joinProjB->setPosition("10%", "20%");
  joinProjB->setSize("80%", "6%");
  joinProjB->getRenderer()->setBackgroundColor(sf::Color(40, 40, 52));
  joinProjB->getRenderer()->setBackgroundColorHover(sf::Color(55, 55, 70));
  joinProjB->getRenderer()->setTextColor(sf::Color(180, 180, 200));
  joinProjB->getRenderer()->setBorders(1);
  joinProjB->getRenderer()->setBorderColor(sf::Color(60, 60, 80));
  joinProjB->getRenderer()->setRoundedBorderRadius(8);
  joinProjB->onPress(&MenuView::joinProj, this, rightPanel);
  rightPanel->add(joinProjB);

  auto importProjB = tgui::Button::create("Importer un projet");
  importProjB->setPosition("10%", "30%");
  importProjB->setSize("80%", "6%");
  importProjB->getRenderer()->setBackgroundColor(sf::Color(99, 102, 241));
  importProjB->getRenderer()->setBackgroundColorHover(sf::Color(118, 120, 255));
  importProjB->getRenderer()->setTextColor(sf::Color::White);
  importProjB->getRenderer()->setBorders(1);
  importProjB->getRenderer()->setBorderColor(sf::Color(60, 60, 80));
  importProjB->getRenderer()->setRoundedBorderRadius(8);
  importProjB->onPress(&MenuView::importProj, this, rightPanel);
  rightPanel->add(importProjB, "importProj");

  if (shareToken != "FFFFF") {
    rightPanel->add(displayToken());
  }
}

void MenuView::importProj(tgui::Panel::Ptr panel) {
  auto &gui = app_.getGui();
  auto &manager = app_.getNetwork();

  // Toggle
  if (panel->get("joinPanel"))
    panel->remove(panel->get("joinPanel"));
  if (panel->get("importPanel")) {
    panel->remove(panel->get("importPanel"));
    return;
  } else {
    auto importPanel = tgui::Panel::create();
    importPanel->setSize("80%", "45%");
    importPanel->setPosition("10%", "40%");
    importPanel->getRenderer()->setBackgroundColor(sf::Color(35, 35, 40));
    importPanel->getRenderer()->setRoundedBorderRadius(8);
    panel->add(importPanel, "importPanel");

    auto infoLabel = tgui::Label::create(
        "⚠ Mettre le projet .natif à importer dans le dossier import");
    infoLabel->setPosition("5%", "5%");
    infoLabel->getRenderer()->setTextColor(sf::Color(180, 180, 200));
    infoLabel->getRenderer()->setTextSize(24);
    importPanel->add(infoLabel);

    auto separator = tgui::Panel::create();
    separator->setSize("80%", "1");
    separator->setPosition("10%", "15%");
    separator->getRenderer()->setBackgroundColor(tgui::Color(55, 55, 70));
    importPanel->add(separator);

    auto fileLabel =
        tgui::Label::create("Entrez le nom du fichier (sans .natif) :");
    fileLabel->setPosition("5%", "22%");
    fileLabel->getRenderer()->setTextColor(sf::Color(180, 180, 200));
    fileLabel->getRenderer()->setTextSize(30);
    importPanel->add(fileLabel);

    auto fileName = tgui::EditBox::create();
    fileName->setDefaultText("Fichier...");
    fileName->getRenderer()->setTextSize(40);
    fileName->getRenderer()->setRoundedBorderRadius(8);
    fileName->setSize("90%", "12%");
    fileName->setPosition("5%", "32%");
    importPanel->add(fileName);

    auto projLabel = tgui::Label::create("Entrez le nom du futur Projet :");
    projLabel->setPosition("5%", "52%");
    projLabel->getRenderer()->setTextColor(sf::Color(180, 180, 200));
    projLabel->getRenderer()->setTextSize(30);
    importPanel->add(projLabel);

    auto projName = tgui::EditBox::create();
    projName->setDefaultText("Nouveau projet...");
    projName->getRenderer()->setTextSize(40);
    projName->getRenderer()->setRoundedBorderRadius(8);
    projName->setSize("90%", "12%");
    projName->setPosition("5%", "62%");
    importPanel->add(projName);

    auto valid = tgui::Button::create("Valider");
    valid->setSize("40%", "12%");
    valid->setPosition("30%", "82%");
    valid->getRenderer()->setBackgroundColor(sf::Color(99, 102, 241));
    valid->getRenderer()->setBackgroundColorHover(sf::Color::Cyan);
    valid->getRenderer()->setTextColor(sf::Color::White);
    valid->getRenderer()->setBorders(0);
    valid->getRenderer()->setRoundedBorderRadius(8);
    importPanel->add(valid);

    valid->onPress([this, panel, importPanel, fileName, projName, &manager]() {
      if (!fileName->getText().empty() && !projName->getText().empty()) {
        std::string path = "../import/" +
                           static_cast<std::string>(fileName->getText()) +
                           ".natif";
        std::cout << "Importation de " << fileName->getText() << " vers "
                  << projName->getText() << std::endl;

        manager.importProj(path, static_cast<std::string>(projName->getText()));

        panel->remove(importPanel);

      } else {
        if (fileName->getText().empty())
          boxError(fileName);
        if (projName->getText().empty())
          boxError(projName);
      }
    });
  }
}

void MenuView::clearProjList() {
  projectList.clear();
  init();
}

void MenuView::handleEvents(const sf::Event &event) {
  auto &gui = app_.getGui();
  if (const auto *mouseClick = event.getIf<sf::Event::MouseButtonPressed>()) {
    auto popup = gui.get("popup");
    // If popup exists
    if (popup) {
      // Gets position of where menu pops
      sf::Vector2f clickPos(mouseClick->position.x, mouseClick->position.y);
      // If click outside the menu, then remove it
      if (!popup->isMouseOnWidget(clickPos)) {
        closePopup();
      }
    }
  }
  if (event.getIf<sf::Event::MouseWheelScrolled>()) {
    auto popup = gui.get("popup");
    if (popup) {
      gui.remove(popup);
    }
  }
}
void MenuView::joinProj(tgui::Panel::Ptr panel) {
  // Toggle
  if (panel->get("importPanel"))
    panel->remove(panel->get("importPanel"));
  if (panel->get("joinPanel")) {
    panel->remove(panel->get("joinPanel"));
    return;
  } else {
    auto &manager = app_.getNetwork();
    auto joinPanel = tgui::Panel::create();
    joinPanel->setSize("80%", "30%");
    joinPanel->setPosition("10%", "40%");
    joinPanel->getRenderer()->setBackgroundColor(sf::Color(35, 35, 40));
    joinPanel->getRenderer()->setRoundedBorderRadius(8);
    panel->add(joinPanel, "joinPanel");

    // Title
    auto title = tgui::Label::create("Rejoindre un Projet");
    title->setPosition("25%", "3%");
    // title->setTextSize(10);
    title->getRenderer()->setTextColor(sf::Color(180, 180, 200));
    joinPanel->add(title);

    // Enter Token
    auto enterTok = tgui::Label::create("Entrez un token");
    enterTok->setPosition("2.5%", "30%");
    enterTok->getRenderer()->setTextSize(40);
    enterTok->getRenderer()->setTextColor(sf::Color(180, 180, 200));
    joinPanel->add(enterTok);

    // TokenBox
    auto token = tgui::EditBox::create();
    token->setDefaultText("Token");
    token->getRenderer()->setTextSize(40);
    token->getRenderer()->setRoundedBorderRadius(8);
    token->setSize("80%", "15%");
    token->setPosition("2.5%", "45%");
    joinPanel->add(token);

    // Valid Button
    auto valid = tgui::Button::create("Valider");
    valid->setSize("40%", "12%");
    valid->setPosition("30%", "75%");
    valid->getRenderer()->setBackgroundColor(sf::Color(99, 102, 241));
    valid->getRenderer()->setBackgroundColorHover(sf::Color::Cyan);
    valid->getRenderer()->setTextColor(sf::Color::White);
    valid->getRenderer()->setBorders(0);
    valid->getRenderer()->setRoundedBorderRadius(8);
    joinPanel->add(valid);
    valid->onPress([this, panel, joinPanel, token, &manager]() {
      // NOTE: need to add verification from server
      if (!token->getText().empty()) {
        manager.joinProject(token->getText().toStdString());
        panel->remove(joinPanel);
      } else
        boxError(token);
    });
  }
}
void MenuView::shareProj() {}
void MenuView::displayProjList(tgui::Panel::Ptr parent) {
  auto panel = tgui::ScrollablePanel::create();
  panel->setPosition("5%", "18%");
  panel->setSize("90%", "60%");
  panel->getRenderer()->setBackgroundColor(sf::Color::Transparent);
  panel->getRenderer()->setBorders(0);
  parent->add(panel, "projPanel");
  panel->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Automatic);

  for (int i = 0; i < (int)projectList.size(); i++) {
    auto row = tgui::Panel::create();
    row->setSize("96%", 56);
    row->setPosition("2%", i * 64);
    row->getRenderer()->setBackgroundColor(sf::Color(32, 32, 40));
    row->getRenderer()->setBorders(0);
    row->getRenderer()->setRoundedBorderRadius(8);
    panel->add(row);

    auto label = tgui::Label::create(projectList[i].projectName);
    label->setPosition(58, "center");
    label->setTextSize(15);
    label->getRenderer()->setTextColor(sf::Color(220, 220, 235));
    row->add(label);

    auto btn = tgui::Button::create("•••");
    btn->setSize(80, 35);
    btn->setPosition("100% -90", "center + 10");
    btn->getRenderer()->setBackgroundColor(sf::Color::Transparent);
    btn->getRenderer()->setBackgroundColorHover(sf::Color(55, 55, 70));
    btn->getRenderer()->setTextColor(sf::Color(140, 140, 160));
    btn->getRenderer()->setBorders(0);
    btn->getRenderer()->setRoundedBorderRadius(6);
    row->add(btn, "BtnMore");
    btn->onPress(&MenuView::showProjectMenu, this, projectList[i], btn);
    btn->setTextSize(20);
  }
}

void MenuView::showProjectMenu(ProjectData project, tgui::Button::Ptr toHover) {
  auto &gui = app_.getGui();
  auto &manager = app_.getNetwork();
  long long id = project.projectId;
  activeMoreButton = toHover;
  sf::Vector2f btnPos = toHover->getAbsolutePosition();
  activeMoreButton->getRenderer()->setBackgroundColor(sf::Color(55, 55, 70));
  auto menu = tgui::ListBox::create();
  menu->getScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  menu->getRenderer()->setBorders(2);
  menu->getRenderer()->setBorderColor(sf::Color::White);
  menu->getRenderer()->setBackgroundColor(sf::Color(40, 40, 52));
  menu->getRenderer()->setTextColor(tgui::Color::White);
  menu->addItem("Ouvrir");
  menu->addItem("Dupliquer");
  menu->addItem("Partager");
  menu->addItem("Quitter");
  menu->addItem("Membres");
  menu->addItem("Exporter");

  if (project.role >= 1) {
    menu->addItem("Renommer");
  }
  if (project.role == 2) {
    menu->addItem("Supprimer");
  }
  float menuHeight = menu->getItemCount() * 45;
  menu->setSize(150, menuHeight);
  menu->setItemHeight(45);

  menu->setPosition(btnPos.x - 150, btnPos.y);

  gui.add(menu, "popup");
  menu->setTextSize(20);
  int8_t projectRole = project.role;
  app_.setCurrentProjRole(projectRole);
  menu->onItemSelect([this, menu, id, toHover, project, projectRole, &manager,
                      &gui](const tgui::String &item) {
    // Delete Action
    if (item == "Supprimer") {
      deleteProject(id);
      // Open Action
    } else if (item == "Ouvrir") {
      manager.getProjectData(id);

      // Rename Action
    } else if (item == "Renommer") {
      // NOTE: Sending project reference to server with "project"
      initInputWidget(focusPopup::RENAME, project);
    } else if (item == "Dupliquer") {
      initInputWidget(focusPopup::DUPLICATE, project);

    } else if (item == "Partager") {
      initInputWidget(focusPopup::TOKEN, project);
    }

    else if (item == "Quitter") {
      Transferring = true;
      initInputWidget(focusPopup::QUIT, project);
    } else if (item == "Membres") {
      initInputWidget(focusPopup::MEMBER, project);

    } else if (item == "Exporter") {
      initInputWidget(focusPopup::EXPORT, project);
    }
    activeMoreButton->getRenderer()->setBackgroundColor(
        tgui::Color::Transparent);

    gui.remove(menu);
    // toHover->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  });
}

void MenuView::deleteProject(long long id) {
  auto &manager = app_.getNetwork();
  auto it =
      std::find_if(projectList.begin(), projectList.end(),
                   [id](const ProjectData &p) { return p.projectId == id; });

  if (it != projectList.end()) {
    projectList.erase(it);
    init();
  }
  manager.delProject(id);
}

void MenuView::closePopup() {
  auto &gui = app_.getGui();
  auto popup = gui.get("popup");
  gui.remove(popup);
  if (activeMoreButton) {
    activeMoreButton->getRenderer()->setBackgroundColor(
        tgui::Color::Transparent);
    activeMoreButton = nullptr;
  }
}

void MenuView::exitAction(tgui::Panel::Ptr background) {
  auto &gui = app_.getGui();
  auto back = std::dynamic_pointer_cast<tgui::Panel>(background);
  auto pan = back->get("Panel");
  gui.remove(pan);
  gui.remove(back);
}

void MenuView::popupRename(tgui::Panel::Ptr renameBackground,
                           ProjectData project, focusPopup view) {
  auto &gui = app_.getGui();
  auto &manager = app_.getNetwork();
  gui.add(renameBackground, "rbackground");

  // Box for project's renamePanel
  auto renamePanel = tgui::Panel::create();
  renamePanel->setSize("35%", "30%");
  renamePanel->setPosition("50% - 17.5%", "50% - 10%");
  renamePanel->getRenderer()->setBackgroundColor(tgui::Color(40, 40, 40));
  renameBackground->add(renamePanel, "Panel");
  // Title
  std::string oldName = project.projectName;
  tgui::Label::Ptr rtitle;
  if (view == focusPopup::RENAME)
    rtitle = tgui::Label::create("Renommer le projet: \n " + oldName);
  else if (view == focusPopup::DUPLICATE)
    rtitle = tgui::Label::create("Dupliquer le projet: \n " + oldName);
  rtitle->getRenderer()->setTextColor(tgui::Color::White);
  rtitle->setPosition("25%", "5%");
  rtitle->setTextSize(24);
  renamePanel->add(rtitle);
  // Rename Box
  auto rname = tgui::EditBox::create();
  rname->setSize("80%", "12%");
  rname->setPosition("10%", "35%");
  rname->setDefaultText("Choisissez un nouveau nom ");
  rname->getRenderer()->setRoundedBorderRadius(8);
  rname->getRenderer()->setBackgroundColor(tgui::Color(28, 28, 36));
  rname->getRenderer()->setBorderColor(sf::Color(55, 55, 70));
  rname->getRenderer()->setBackgroundColorFocused(tgui::Color::White);
  renamePanel->add(rname);
  // Valid Button
  auto rvalid = tgui::Button::create("Valider");
  rvalid->setSize("40%", "12%");
  rvalid->setPosition("30%", "75%");
  rvalid->getRenderer()->setBackgroundColor(sf::Color(99, 102, 241));
  rvalid->getRenderer()->setBackgroundColorHover(sf::Color(118, 120, 255));
  rvalid->getRenderer()->setTextColor(sf::Color::White);
  rvalid->getRenderer()->setBorders(0);
  rvalid->getRenderer()->setRoundedBorderRadius(8);
  renamePanel->add(rvalid);
  // Exit Button
  auto exitB = tgui::Button::create("✕");
  exitB->setSize(30, 30);
  exitB->setPosition("100% - 35", "5");
  exitB->getRenderer()->setBackgroundColor(sf::Color::Transparent);
  exitB->getRenderer()->setTextColor(sf::Color::White);
  exitB->getRenderer()->setBackgroundColorHover(sf::Color(255, 100, 100));
  renamePanel->add(exitB);
  exitB->onPress([this, renameBackground]() { exitAction(renameBackground); });

  rvalid->onPress(
      [this, rname, project, renameBackground, view, &gui, &manager]() {
        if (view == focusPopup::RENAME) {
          std::string newName = static_cast<std::string>(rname->getText());
          if (!newName.empty()) {
            manager.renameProject(project.projectId, newName);
            exitAction(renameBackground);
          } else
            (boxError(rname));
        } else if (view == focusPopup::DUPLICATE) {
          std::string newName = static_cast<std::string>(rname->getText());
          if (!newName.empty()) {
            manager.dupProj(project.projectId, newName);
            exitAction(renameBackground);
          } else
            (boxError(rname));
        }
      });
}
void MenuView::updateList() { init(); }

void MenuView::initInputWidget(focusPopup focus, ProjectData project) {
  // Grey Backgroung
  auto background = tgui::Panel::create();
  background->setSize("100%", "100%");
  background->getRenderer()->setBackgroundColor({40, 40, 40, 150});
  // gui.add(background, "background");
  background->setTextSize(30);
  background->moveToFront();
  background->setFocused(true);
  // Box for project's renamePanel

  switch (focus) {
  case (focusPopup::CREATE): {
    popupCreate(background);
    // popupCreate();
    break;
  }
  case (focusPopup::RENAME): {
    popupRename(background, project, focusPopup::RENAME);
    // popupRename();
    break;
  }
  case (focusPopup::DUPLICATE): {
    popupRename(background, project, focusPopup::DUPLICATE);
    break;
  }
  case (focusPopup::TOKEN): {
    popupCreateToken(background, project);
    break;
  }
  case (focusPopup::MEMBER): {
    activeProjectData_ = project;
    activeBackground_ = background;
    Transferring = false;
    app_.getGui().add(background, "memberBackground");
    app_.getNetwork().getUsersProjects(project.projectId);
    break;
  }
  case (focusPopup::QUIT): {
    activeProjectData_ = project;
    activeBackground_ = background;
    app_.getGui().add(background, "memberBackground");
    app_.getNetwork().getUsersProjects(project.projectId);

    break;
  }
  case (focusPopup::EXPORT): {
    popupExport(background, project);
    break;
  }
  }
}
void MenuView::popupExport(tgui::Panel::Ptr background, ProjectData project) {
  auto &gui = app_.getGui();
  auto &manager = app_.getNetwork();
  gui.add(background);

  auto data = tgui::Panel::create();
  data->setSize("30%", "35%");
  // Trying to center with half the size of data (above)
  data->setPosition("50%-15%", "50%-17.5%");
  data->getRenderer()->setBackgroundColor(tgui::Color(28, 28, 36));
  data->getRenderer()->setRoundedBorderRadius(12);
  data->getRenderer()->setBorders(1);
  data->getRenderer()->setBorderColor(tgui::Color(55, 55, 70));
  background->add(data, "ExportPanel");

  auto exitB = tgui::Button::create("✕");
  exitB->setSize("10%", "10%");
  exitB->setPosition("88%", "3%");
  exitB->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  exitB->getRenderer()->setBackgroundColorHover(tgui::Color(60, 60, 75));
  exitB->getRenderer()->setTextColor(tgui::Color(160, 160, 180));
  exitB->getRenderer()->setTextColorHover(tgui::Color::White);
  exitB->getRenderer()->setBorders(0);
  exitB->getRenderer()->setRoundedBorderRadius(6);
  data->add(exitB);
  exitB->onPress([this, background]() { exitAction(background); });

  auto title = tgui::Label::create("Choisir le type d'export");
  title->setPosition("0%", "10%");
  title->setSize("100%", "15%");
  title->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
  title->getRenderer()->setTextColor(sf::Color::White);
  title->setTextSize(22);
  data->add(title);

  auto imgBtn = tgui::Button::create("Format Image\n(PNG, JPG...)");
  imgBtn->setSize("40%", "40%");
  imgBtn->setPosition("7.5%", "40%");
  imgBtn->getRenderer()->setBackgroundColor(tgui::Color(45, 45, 55));
  imgBtn->getRenderer()->setTextColor(tgui::Color::White);
  imgBtn->getRenderer()->setRoundedBorderRadius(8);
  data->add(imgBtn);

  auto nativeBtn = tgui::Button::create("Export Natif\n(.natif)");
  nativeBtn->setSize("40%", "40%");
  nativeBtn->setPosition("52.5%", "40%");
  nativeBtn->getRenderer()->setBackgroundColor(tgui::Color(45, 45, 55));
  nativeBtn->getRenderer()->setTextColor(tgui::Color::White);
  nativeBtn->getRenderer()->setRoundedBorderRadius(8);
  data->add(nativeBtn);

  imgBtn->onPress([this, data, project, background, &manager]() {
    imageExport(background, data, project);
  });

  nativeBtn->onPress([this, project, background, &manager]() {
    auto destination =
        pfd::select_folder("Choisissez un dossier de destination").result();
    if (!destination.empty()) {
      std::string path = destination + "/" + project.projectName + ".natif";
      manager.exportToNative(project.projectId, path);
      exitAction(background);
    }
  });
}

void MenuView::imageExport(tgui::Panel::Ptr background, tgui::Panel::Ptr panel,
                           ProjectData project) {
  auto &manager = app_.getNetwork();
  panel->removeAllWidgets();
  auto exitB = tgui::Button::create("✕");
  exitB->setSize("10%", "10%");
  exitB->setPosition("88%", "3%");
  exitB->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  exitB->getRenderer()->setTextColor(tgui::Color::White);
  panel->add(exitB);
  exitB->onPress([this, background]() { exitAction(background); });
  auto title = tgui::Label::create("Exporter le projet");
  title->setPosition("0%", "12%");
  title->getScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  title->setSize("100%", "10%");
  title->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
  title->getRenderer()->setTextColor(sf::Color(255, 255, 255));
  title->setTextSize(22);
  panel->add(title);

  auto separator = tgui::Panel::create();
  separator->setSize("80%", "1");
  separator->setPosition("10%", "25%");
  separator->getRenderer()->setBackgroundColor(tgui::Color(55, 55, 70));
  panel->add(separator);

  auto formatLabel = tgui::Label::create("Format d'exportation :");
  formatLabel->setPosition("10%", "32%");
  formatLabel->getRenderer()->setTextColor(tgui::Color(160, 160, 180));
  formatLabel->setTextSize(14);
  panel->add(formatLabel);

  auto comboBox = tgui::ComboBox::create();
  comboBox->setSize("80%", "13%");
  comboBox->setPosition("10%", "44%");
  comboBox->addItem("PNG");
  comboBox->addItem("JPG");
  comboBox->addItem("BMP");
  comboBox->setSelectedItem("PNG");
  comboBox->getRenderer()->setBackgroundColor(tgui::Color(40, 40, 52));
  comboBox->getRenderer()->setTextColor(tgui::Color::White);
  comboBox->getRenderer()->setBorderColor(tgui::Color(55, 55, 70));
  comboBox->getRenderer()->setArrowBackgroundColor(tgui::Color(99, 102, 241));
  comboBox->getRenderer()->setArrowColor(tgui::Color::White);
  comboBox->getRenderer()->setBorders(1);
  comboBox->getRenderer()->setRoundedBorderRadius(8);
  panel->add(comboBox);

  auto valid = tgui::Button::create("Générer l'image");
  valid->setSize("80%", "13%");
  valid->setPosition("10%", "78%");
  valid->getRenderer()->setBackgroundColor(tgui::Color(99, 102, 241));
  valid->getRenderer()->setBackgroundColorHover(tgui::Color(118, 120, 255));
  valid->getRenderer()->setTextColor(tgui::Color::White);
  valid->getRenderer()->setBorders(0);
  valid->getRenderer()->setRoundedBorderRadius(8);
  panel->add(valid);

  valid->onPress([this, comboBox, project, background, &manager]() {
    std::string extension = comboBox->getSelectedItem().toStdString();
    std::transform(extension.begin(), extension.end(), extension.begin(),
                   ::tolower);

    app_.setIsExporting(true, extension);
    manager.getProjectData(project.projectId);

    exitAction(background);
  });
}
void MenuView::popupQuit(tgui::Panel::Ptr back, ProjectData project,
                         focusPopup view) {

};

void MenuView::setAllUsers(std::vector<MemberEntry> users) {
  allUsers_ = users;
  // TODO: Manage member from the menu
  displayMemberList();
  Transferring = false;
}

void MenuView::displayMemberList() {
  auto &gui = app_.getGui();
  auto &manager = app_.getNetwork();
  User &currentUser = app_.getUser();

  // If owner is alone, he can leave project instantly
  if (Transferring && allUsers_.size() == 1 &&
      allUsers_[0].userId == currentUser.getId()) {
    app_.getNetwork().leaveProject(activeProjectData_.projectId);
    app_.getNetwork().getProjectList();
    app_.changeView(std::make_unique<MenuView>(app_));
    return;
  }

  auto parent = tgui::Panel::create();
  parent->setSize("40%", "60%");
  parent->setPosition("30%", "20%");
  parent->getRenderer()->setBackgroundColor({40, 40, 40, 240});
  parent->getRenderer()->setRoundedBorderRadius(10);
  gui.get<tgui::Panel>("memberBackground")->add(parent, "Panel");

  // Title

  auto title =
      tgui::Label::create(Transferring ? "Choisissez un nouveau propriétaire"
                                       : "Membres du projet");
  title->setPosition("center", "5%");
  title->setTextSize(24);
  title->getRenderer()->setTextColor(sf::Color::White);
  parent->add(title);

  auto exitB = tgui::Button::create("✕");
  exitB->setSize(30, 30);
  exitB->setPosition("100% - 35", "5");
  exitB->getRenderer()->setBackgroundColor(sf::Color::Transparent);
  exitB->getRenderer()->setTextColor(sf::Color::White);
  exitB->getRenderer()->setBackgroundColorHover(sf::Color(255, 100, 100));
  parent->add(exitB);

  exitB->onPress([this]() {
    exitAction(app_.getGui().get<tgui::Panel>("memberBackground"));
  });

  auto panel = tgui::ScrollablePanel::create();
  panel->setPosition("5%", "18%");
  panel->setSize("90%", "75%");
  panel->getRenderer()->setBackgroundColor(sf::Color::Transparent);
  panel->getRenderer()->setBorders(0);
  panel->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Automatic);
  parent->add(panel);

  for (int i = 0; i < (int)allUsers_.size(); i++) {
    if (allUsers_[i].pseudo == currentUser.getUser()) {
      continue;
    }
    auto row = tgui::Panel::create();
    row->setSize("96%", 56);
    row->setPosition("2%", i * 64);
    row->getRenderer()->setBackgroundColor(sf::Color(32, 32, 40));
    row->getRenderer()->setRoundedBorderRadius(8);
    panel->add(row);

    // Pseudo
    std::string pseudo = allUsers_[i].pseudo;
    int8_t userId = allUsers_[i].userId;
    auto label = tgui::Label::create(pseudo);
    label->setPosition(20, "center");
    label->setTextSize(18);
    label->getRenderer()->setTextColor(sf::Color(220, 220, 235));
    row->add(label);

    // Clickable
    auto clickable = tgui::Button::create();
    clickable->setSize("100%", "100%");
    clickable->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
    clickable->getRenderer()->setBackgroundColorHover(
        tgui::Color(255, 255, 255, 20));
    clickable->getRenderer()->setRoundedBorderRadius(8);
    clickable->onPress([this, pseudo, userId, &manager] {
      manager.changeRole(userId, activeProjectData_.projectId, 2);
      app_.getNetwork().leaveProject(activeProjectData_.projectId);
      app_.getNetwork().getProjectList();
      app_.changeView(std::make_unique<MenuView>(app_));
    });

    std::string nomRole = (allUsers_[i].role == 1) ? "Editeur" : "Spectateur";
    if (allUsers_[i].role == 2)
      nomRole = "Propriétaire";

    auto labelRole = tgui::Label::create(nomRole);
    if (Transferring) {
      labelRole->setPosition("70%", "center");
      labelRole->setTextSize(14);
      labelRole->getRenderer()->setTextColor(sf::Color(140, 140, 160));
      row->add(labelRole);
      row->add(clickable);

    } else {
      // Adding more button
      if (activeProjectData_.role == 2) {
        labelRole->setPosition("62.5%", "center");
        auto btn = tgui::Button::create("•••");
        btn->setSize(80, 35);
        btn->setPosition("100% -90", "center + 10");
        btn->getRenderer()->setBackgroundColor(sf::Color::Transparent);
        btn->getRenderer()->setBackgroundColorHover(sf::Color(55, 55, 70));
        btn->getRenderer()->setTextColor(sf::Color(140, 140, 160));
        btn->getRenderer()->setBorders(0);
        btn->getRenderer()->setRoundedBorderRadius(6);
        row->add(labelRole);
        row->add(btn, "BtnMore");
        btn->onPress(&MenuView::showUserManagment, this, btn, i,
                     activeProjectData_);
        btn->setTextSize(20);
      } else {
        labelRole->setPosition("70%", "center");
        row->add(labelRole);
      }
    }
  }
}

void MenuView::showUserManagment(tgui::Button::Ptr toHover, int place,
                                 ProjectData project) {
  auto &gui = app_.getGui();
  auto &manager = app_.getNetwork();
  activeMoreButton = toHover;
  sf::Vector2f btnPos = toHover->getAbsolutePosition();
  activeMoreButton->getRenderer()->setBackgroundColor(sf::Color(55, 55, 70));
  auto menu = tgui::ListBox::create();
  menu->getScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  menu->getRenderer()->setBorders(2);
  menu->getRenderer()->setBorderColor(sf::Color::White);
  menu->getRenderer()->setBackgroundColor(sf::Color(40, 40, 52));
  menu->getRenderer()->setTextColor(tgui::Color::White);
  menu->addItem("Promouvoir");
  menu->addItem("Retrograder");
  menu->addItem("Ejecter");
  menu->addItem("Propriétaire");
  float menuHeight = menu->getItemCount() * 45;
  menu->setSize(150, menuHeight);
  menu->setItemHeight(45);

  menu->setPosition(btnPos.x - 150, btnPos.y);

  gui.add(menu, "popup");
  menu->setTextSize(20);
  menu->onItemSelect(
      [this, menu, place, project, &manager, &gui](const tgui::String &item) {
        int8_t role = project.role;
        if (item == "Promouvoir") {
          // Spector -> Editor
          if (allUsers_[place].role == 0) {
            manager.changeRole(allUsers_[place].userId, project.projectId, 1);
          } else if (role == 1) {
            // WARNING YOU ARE LOSING YOUR OWNER STATUS
          }

        } else if (item == "Retrograder") {
          if (allUsers_[place].role == 1) {
            manager.changeRole(allUsers_[place].userId, project.projectId, 0);
          } else if (role == 1) {
            // WARNING YOU CAN'T DOWNGRADE A SPECTATOR
          }
        } else if (item == "Ejecter") {
          manager.kickUser(allUsers_[place].userId, project.projectId);
          displayMemberList();

        } else if (item == "Propriétaire") {
          // We find the owner position
          for (int i = 0; i < (int)allUsers_.size(); i++) {
            if (allUsers_[i].pseudo == app_.getUser().getUser()) {
              // We downgrade owner to editor
              manager.changeRole(allUsers_[i].userId, project.projectId, 1);
            }
          }
          // Change target role to owner
          manager.changeRole(allUsers_[place].userId, project.projectId, 2);
        }

        activeMoreButton->getRenderer()->setBackgroundColor(
            tgui::Color::Transparent);
        gui.remove(menu);
      });
}

void MenuView::updateMemberRole(uint projectId, uint targetId, int8_t newRole) {
  // If we are the target
  if (app_.getUser().getId() == targetId) {
    for (auto &p : projectList) {
      if (p.projectId == projectId) {
        p.role = newRole;
        break;
      }
    }
    // Updating "My Project List" we loaded in the background to update the
    // roles
    init();
    return;
  }

  if (activeProjectData_.projectId == projectId) {
    // If member panel opened -> refresh (our role didn't changed)
    for (auto &i : allUsers_) {
      if (i.userId == targetId) {
        i.role = newRole;
      }
    }

    auto &gui = app_.getGui();
    auto bg = gui.get<tgui::Panel>("memberBackground");
    if (bg && bg->get("Panel")) {
      bg->remove(bg->get("Panel"));
      displayMemberList();
    }
  }
}

void MenuView::popupCreate(tgui::Panel::Ptr background) {
  auto &gui = app_.getGui();
  gui.add(background);
  auto data = tgui::Panel::create();
  data->setSize("35%", "40%");
  data->setPosition("50%-17.5%", "50%-20% ");
  data->getRenderer()->setBackgroundColor(tgui::Color(40, 40, 40));
  background->add(data, "Panel");

  auto title = tgui::Label::create("Nouveau projet");
  title->getRenderer()->setTextColor(tgui::Color::White);
  title->setPosition("37%", "5%");
  title->setTextSize(50);
  data->add(title);

  auto name = tgui::EditBox::create();
  name->setSize("80%", "12%");
  name->setPosition("10%", "20%");
  name->setDefaultText("Nom: ...");
  name->getRenderer()->setRoundedBorderRadius(8);
  name->getRenderer()->setBackgroundColor(tgui::Color(28, 28, 36));
  name->getRenderer()->setBorderColor(sf::Color(55, 55, 70));
  name->getRenderer()->setBackgroundColorFocused(tgui::Color::White);
  data->add(name);

  auto scale = tgui::EditBox::create();
  scale->setInputValidator("[0-9]*");
  scale->setMaximumCharacters(5);
  scale->setSize("80%", "12%");
  scale->setPosition("10%", "42%");
  scale->setDefaultText("Echelle: ...");
  scale->getRenderer()->setRoundedBorderRadius(8);
  scale->getRenderer()->setBackgroundColor(tgui::Color(28, 28, 36));
  scale->getRenderer()->setBorderColor(sf::Color(55, 55, 70));
  scale->getRenderer()->setBackgroundColorFocused(tgui::Color::White);
  data->add(scale);

  auto sizePx = tgui::EditBox::create();
  sizePx->setInputValidator("[0-9]*");
  sizePx->setMaximumCharacters(5);
  sizePx->setSize("80%", "12%");
  sizePx->setPosition("10%", "64%");
  sizePx->setDefaultText("Taille (x): ...");
  sizePx->getRenderer()->setRoundedBorderRadius(8);
  sizePx->getRenderer()->setBackgroundColor(tgui::Color(28, 28, 36));
  sizePx->getRenderer()->setBorderColor(sf::Color(55, 55, 70));
  sizePx->getRenderer()->setBackgroundColorFocused(tgui::Color::White);
  data->add(sizePx);

  auto sizePy = tgui::EditBox::create();
  sizePy->setInputValidator("[0-9]*");
  sizePy->setMaximumCharacters(5);
  sizePy->setSize("40%", "12%");
  sizePy->setPosition("50%", "64%");
  sizePy->setDefaultText("Taille (y): ...");
  sizePy->getRenderer()->setRoundedBorderRadius(8);
  sizePy->getRenderer()->setBackgroundColor(tgui::Color(28, 28, 36));
  sizePy->getRenderer()->setBorderColor(sf::Color(55, 55, 70));
  sizePy->getRenderer()->setBackgroundColorFocused(tgui::Color::White);
  data->add(sizePy);

  // Exit Button
  auto exitB = tgui::Button::create("✕");
  exitB->setSize(30, 30);
  exitB->setPosition("100% - 35", "5");
  exitB->getRenderer()->setBackgroundColor(sf::Color::Transparent);
  exitB->getRenderer()->setTextColor(sf::Color::White);
  exitB->getRenderer()->setBackgroundColorHover(sf::Color(255, 100, 100));
  data->add(exitB);
  exitB->onPress([this, background]() { exitAction(background); });

  // Valid Button
  auto valid = tgui::Button::create("Valider");
  valid->setSize("40%", "12%");
  valid->setPosition("30%", "90% - 20");
  valid->getRenderer()->setBackgroundColor(sf::Color(99, 102, 241));
  valid->getRenderer()->setBackgroundColorHover(sf::Color(118, 120, 255));
  valid->getRenderer()->setTextColor(sf::Color::White);
  valid->getRenderer()->setBorders(0);
  valid->getRenderer()->setRoundedBorderRadius(8);
  data->add(valid);
  valid->onPress([this, scale, sizePx, sizePy, name]() {
    // Les getText() sont appelés au moment du clic !
    if (checkInput(scale, sizePx, sizePy))
      this->createProj(scale->getText(), sizePx->getText(), sizePy->getText(),
                       name->getText(), 2, app_.getWindow(), app_.getGui());
  });
}

bool MenuView::boxError(tgui::EditBox::Ptr box) {
  box->getRenderer()->setBorderColor(sf::Color::Red);
  box->getRenderer()->setDefaultTextColor(sf::Color::Red);
  box->setText("");
  box->setDefaultText("Changer la donnée");
  return false;
}
bool MenuView::checkInput(tgui::EditBox::Ptr scale, tgui::EditBox::Ptr sizePx,
                          tgui::EditBox::Ptr sizePy) {
  bool res = true;
  if (scale->getText().empty() || scale->getText().toUInt() == 0) {
    res = boxError(scale);
  }
  if (sizePx->getText().empty() || sizePx->getText().toUInt() == 0) {
    res = boxError(sizePx);
  }
  if (sizePy->getText().empty() || sizePy->getText().toUInt() == 0) {
    res = boxError(sizePy);
  }
  return res;
}

void MenuView::createProj(tgui::String scale, tgui::String sizeX,
                          tgui::String sizeY, tgui::String name,
                          unsigned int id, sf::RenderWindow &window,
                          tgui::Gui &gui) {
  auto &manager = app_.getNetwork();
  // NOTE: Need verification if data isn't empty
  // NOTE: Check with server (chinsou tomas)
  unsigned int scaleInt = scale.toUInt();
  sf::Vector2u size = {sizeX.toUInt(), sizeY.toUInt()};
  std::string nameS = name.toStdString();
  // Notify the server that a Proj is being created by using
  // ClientNetworkManager
  manager.createProject(nameS, size, scaleInt);
  gui.removeAllWidgets();

  // Role = 2 bc owner
  app_.getProject() = std::make_unique<Project>(
      scaleInt, size, nameS, id, app_.getWindow(), gui, app_.getNetwork(), 2);

  projectList.push_back(getProjectData(app_.getProject()));

  resetShareToken();
  app_.changeView(std::make_unique<GameView>(app_));
}

ProjectData MenuView::askProjectData() {
  initInputWidget(focusPopup::CREATE);
  return ProjectData{};
}

ProjectData MenuView::getProjectData(std::unique_ptr<Project> &newProj) {
  return ProjectData{newProj->getId(), newProj->getRole(), newProj->getName()};
}

void MenuView::updateCreatedProjectId(uint32_t projId) {
  projectList.back().projectId = projId;
}

void MenuView::addProjectList(ProjectData project) {
  projectList.push_back(project);
  init();
}

void MenuView::updateProjectNameInList(long long id, const std::string &name) {
  for (auto &projData : projectList) {
    if (projData.projectId == id) {
      projData.projectName = name;
      break;
    }
  }
  // Refreshing project list
  init();
}
void MenuView::setShareToken(std::string newToken) {
  shareToken = newToken;
  init();
}

void MenuView::resetShareToken() { shareToken = "FFFFF"; }

void MenuView::generateToken(uint8_t role, uint id) {
  auto &manager = app_.getNetwork();
  // Sending message to create Code
  manager.createProjectCode(role, id);
}

tgui::Panel::Ptr MenuView::displayToken() {
  auto tokenPanel = tgui::Panel::create();
  tokenPanel->setSize("80%", "15%");
  tokenPanel->setPosition("10%", "32%");
  tokenPanel->getRenderer()->setBackgroundColor(tgui::Color(35, 35, 48));
  tokenPanel->getRenderer()->setBorders(1);
  tokenPanel->getRenderer()->setBorderColor(tgui::Color(99, 102, 241));
  tokenPanel->getRenderer()->setRoundedBorderRadius(10);

  auto tokenTitle = tgui::Label::create("Token à partager :");
  tokenTitle->setPosition("5%", "10%");
  tokenTitle->setTextSize(13);
  tokenTitle->getRenderer()->setTextColor(tgui::Color(160, 160, 180));
  tokenPanel->add(tokenTitle);

  auto tokenValue = tgui::Label::create(shareToken);
  tokenValue->setPosition("5%", "55%");
  tokenValue->setSize("90%", "35%");
  tokenValue->setTextSize(14);
  tokenValue->getRenderer()->setTextColor(tgui::Color(99, 102, 241));
  tokenPanel->add(tokenValue);

  auto tokenCopy = tgui::Button::create();
  tokenCopy->setSize("10%", "40%");
  tokenCopy->setPosition("83%", " 25%");
  tokenCopy->getRenderer()->setTexture("../res/images/draft.png");
  tokenCopy->getRenderer()->setTextSize(16);
  tokenCopy->getRenderer()->setTextColor(tgui::Color::White);
  tokenCopy->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  tokenCopy->getRenderer()->setBackgroundColorHover(tgui::Color(55, 55, 70));
  tokenCopy->getRenderer()->setBorders(0);
  tokenCopy->getRenderer()->setRoundedBorderRadius(6);

  // Supreposition
  tokenCopy->getRenderer()->setTextureHover("../res/images/draft_hover.png");
  //
  tokenPanel->add(tokenCopy);
  tokenCopy->onPress([this]() { sf::Clipboard::setString(shareToken); });

  return tokenPanel;
}

void MenuView::popupCreateToken(tgui::Panel::Ptr background,
                                ProjectData project) {
  auto &gui = app_.getGui();
  auto &manager = app_.getNetwork();
  gui.add(background);

  // Panel principal
  auto data = tgui::Panel::create();
  data->setSize("30%", "35%");
  data->setPosition("50%-15%", "50%-17.5%");
  data->getRenderer()->setBackgroundColor(tgui::Color(28, 28, 36));
  data->getRenderer()->setRoundedBorderRadius(12);
  data->getRenderer()->setBorders(1);
  data->getRenderer()->setBorderColor(tgui::Color(55, 55, 70));
  background->add(data, "TokenPanel");

  // Exit button
  auto exitB = tgui::Button::create("✕");
  exitB->setSize("10%", "10%");
  exitB->setPosition("88%", "3%");
  exitB->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  exitB->getRenderer()->setBackgroundColorHover(tgui::Color(60, 60, 75));
  exitB->getRenderer()->setTextColor(tgui::Color(160, 160, 180));
  exitB->getRenderer()->setTextColorHover(tgui::Color::White);
  exitB->getRenderer()->setBorders(0);
  exitB->getRenderer()->setRoundedBorderRadius(6);
  data->add(exitB);
  exitB->onPress([this, background]() { exitAction(background); });

  // Title
  auto title = tgui::Label::create("Partager le projet");
  title->setPosition("0%", "16%");
  title->setSize("100%", "10%");
  title->getScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  title->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
  title->getRenderer()->setTextColor(sf::Color(255, 255, 255));
  title->setTextSize(22);
  data->add(title);

  // Fancy separator
  auto separator = tgui::Panel::create();
  separator->setSize("80%", "1");
  separator->setPosition("10%", "25%");
  separator->getRenderer()->setBackgroundColor(tgui::Color(55, 55, 70));
  data->add(separator);

  // Role
  auto roleLabel = tgui::Label::create("Rôle à partager");
  roleLabel->setPosition("10%", "32%");
  roleLabel->getRenderer()->setTextColor(tgui::Color(160, 160, 180));
  roleLabel->setTextSize(14);
  data->add(roleLabel);

  // ListBox
  auto comboBox = tgui::ComboBox::create();
  comboBox->setSize("80%", "13%");
  comboBox->setPosition("10%", "44%");
  comboBox->addItem("Spectateur");
  if (project.role > 0) {
    comboBox->addItem("Editeur");
  }
  comboBox->setSelectedItem("Spectateur");
  comboBox->getRenderer()->setBackgroundColor(tgui::Color(40, 40, 52));
  // comboBox->getRenderer()->setBackgroundColorHover(tgui::Color(50, 50, 65));
  comboBox->getRenderer()->setTextColor(tgui::Color::White);
  comboBox->getRenderer()->setBorderColor(tgui::Color(55, 55, 70));
  comboBox->getRenderer()->setArrowBackgroundColor(tgui::Color(99, 102, 241));
  comboBox->getRenderer()->setArrowColor(tgui::Color::White);
  comboBox->getRenderer()->setBorders(1);
  comboBox->getRenderer()->setRoundedBorderRadius(8);
  data->add(comboBox);

  // comboBox->onItemSelect([this, comboBox](const tgui::String& item){
  //   if (item == "Spectateur"){
  //     comboBox->setSelectedItem("Spectateur");
  //   }
  //   else if (item == "Editeur"){
  //     comboBox->setSelectedItem("Editeur");
  //
  //   }
  // });

  // Bouton valider
  auto valid = tgui::Button::create("Générer le token");
  valid->setSize("80%", "13%");
  valid->setPosition("10%", "78%");
  valid->getRenderer()->setBackgroundColor(tgui::Color(99, 102, 241));
  valid->getRenderer()->setBackgroundColorHover(tgui::Color(118, 120, 255));
  valid->getRenderer()->setTextColor(tgui::Color::White);
  valid->getRenderer()->setBorders(0);
  valid->getRenderer()->setRoundedBorderRadius(8);
  data->add(valid);

  valid->onPress([this, comboBox, project, background, &gui]() {
    tgui::String selected = comboBox->getSelectedItem();

    if (selected == "Editeur") {
      generateToken(1, project.projectId);

    } else if (selected == "Spectateur") {
      generateToken(0, project.projectId);
    }
    exitAction(background);
  });
}
