#include "Window.hpp"
#include <SFML/Graphics/Color.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include <string>

void Window::initMenuWidget() {
  gui.removeAllWidgets();

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
  createProjB->onPress(&Window::askProjectData, this);

  auto joinProjB = tgui::Button::create("Rejoindre un projet");
  joinProjB->setPosition("10%", "20%");
  joinProjB->setSize("80%", "6%");
  joinProjB->getRenderer()->setBackgroundColor(sf::Color(40, 40, 52));
  joinProjB->getRenderer()->setBackgroundColorHover(sf::Color(55, 55, 70));
  joinProjB->getRenderer()->setTextColor(sf::Color(180, 180, 200));
  joinProjB->getRenderer()->setBorders(1);
  joinProjB->getRenderer()->setBorderColor(sf::Color(60, 60, 80));
  joinProjB->getRenderer()->setRoundedBorderRadius(8);
  joinProjB->onPress(&Window::joinProj, this, rightPanel);
  rightPanel->add(joinProjB);

  // auto shareProjB = tgui::Button::create("Partager un projet");
  // shareProjB->setPosition("10%", "20%");
  // shareProjB->setSize("80%", "6%");
  // shareProjB->getRenderer()->setBackgroundColor(sf::Color(40, 40, 52));
  // shareProjB->getRenderer()->setBackgroundColorHover(sf::Color(55, 55, 70));
  // shareProjB->getRenderer()->setTextColor(sf::Color(180, 180, 200));
  // shareProjB->getRenderer()->setBorders(1);
  // shareProjB->getRenderer()->setBorderColor(sf::Color(60, 60, 80));
  // shareProjB->getRenderer()->setRoundedBorderRadius(8);
  // shareProjB->onPress(&Window::shareProj, this);
  // rightPanel->add(shareProjB);
}
void Window::joinProj(tgui::Panel::Ptr panel) {
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
  valid->onPress([this, panel, joinPanel, token]() {
    // NOTE: need to add verification from server
    if (!token->getText().empty())
      panel->remove(joinPanel);
    else
      boxError(token);
  });
}
void Window::shareProj() {}
void Window::displayProjList(tgui::Panel::Ptr parent) {
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
    btn->onPress(&Window::showProjectMenu, this, projectList[i], btn);
    btn->setTextSize(20);
  }
}

void Window::showProjectMenu(ProjectData project, tgui::Button::Ptr toHover) {
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
  menu->addItem("Supprimer");
  // If Editor or Owner
  // if (project.role == 0 || project.role == 1) {
  menu->addItem("Renommer");
  // }
  menu->addItem("Dupliquer");
  menu->addItem("Partager");
  menu->addItem("Quitter");
  float menuHeight = menu->getItemCount() * 45;
  menu->setSize(150, menuHeight);
  menu->setItemHeight(45);

  menu->setPosition(btnPos.x - 150, btnPos.y);

  gui.add(menu, "popup");
  menu->setTextSize(20);
  // menu->onUnfocus([this, menu]() { gui.remove(menu); });
  menu->onItemSelect(
      [this, menu, id, toHover, project](const tgui::String &item) {
        // Delete Action
        if (item == "Supprimer") {
          auto it = std::find_if(
              projectList.begin(), projectList.end(),
              [id](const ProjectData &p) { return p.projectId == id; });

          if (it != projectList.end()) {
            projectList.erase(it);
            std::cout << "Suppression du projet " << id << std::endl;
            initMenuWidget();
          }
          manager.delProject(id);
          // Open Action
        } else if (item == "Ouvrir") {
          std::cout << "Ouverture du projet " << std::endl;
          manager.getProjectData(id);
      
          
          // Rename Action
        } else if (item == "Renommer") {
          std::cout << "appuyé" << std::endl;
          // NOTE: Sending project reference to server with "project"
          initInputWidget(focusPopup::RENAME, project);
        } else if (item == "Dupliquer") {
          initInputWidget(focusPopup::DUPLICATE, project);
        } else if (item == "Partager") {
        } else if (item == "Quitter") {
        }
        activeMoreButton->getRenderer()->setBackgroundColor(
            tgui::Color::Transparent);

        this->gui.remove(menu);
        // toHover->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
      });
}

void Window::closePopup() {
  auto popup = gui.get("popup");
  gui.remove(popup);
  if (activeMoreButton) {
    activeMoreButton->getRenderer()->setBackgroundColor(
        tgui::Color::Transparent);
    activeMoreButton = nullptr;
  }
}

void Window::exitAction(tgui::Panel::Ptr background) {
  auto back = std::dynamic_pointer_cast<tgui::Panel>(background);
  auto pan = back->get("Panel");
  gui.remove(pan);
  gui.remove(back);
}

void Window::popupRename(tgui::Panel::Ptr renameBackground, ProjectData project,
                         focusPopup view) {
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
  else 
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
  auto exitB = tgui::Button::create("x");
  exitB->setSize("10%", "10%");
  exitB->setPosition("0%", "0%");
  exitB->getRenderer()->setBackgroundColor(sf::Color(102,178,255));
  exitB->getRenderer()->setBackgroundColorHover(sf::Color::White);
  exitB->getRenderer()->setTextColor(sf::Color::Black);
  exitB->getRenderer()->setBorders(0);
  renamePanel->add(exitB);
  exitB->onPress([this, renameBackground]() { exitAction(renameBackground); });

  rvalid->onPress([this, rname, project, renameBackground, view]() {
    if (view == focusPopup::RENAME) {
      std::cout << "Renaming project id: " << project.projectId << std::endl;
      std::string newName = static_cast<std::string>(rname->getText());
      if (!newName.empty()) {
        manager.renameProject(project.projectId, newName);
        exitAction(renameBackground);

      } else
        (boxError(rname));
    } else if (view == focusPopup::DUPLICATE) {
      std::string newName = static_cast<std::string>(rname->getText());
      if (!newName.empty()) {
        std::cout <<"duplicating projects id: " <<project.projectId <<std::endl;
        manager.dupProj(project.projectId, newName);
        exitAction(renameBackground);

      } else
        (boxError(rname));
    }
  });
}
void Window::updateList(){
  initMenuWidget();
}

void Window::initInputWidget(focusPopup focus, ProjectData project) {
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
  }
  }
}
// void Window::popupDuplicate(tgui::Panel::Ptr dbackground, ProjectData
// project) {
//   gui.add(dbackground, "dbackground");
//
//   // Box for project's duplicatePanel
//   auto duplicatePanel = tgui::Panel::create();
//   duplicatePanel->setSize("35%", "30%");
//   duplicatePanel->setPosition("50% - 17.5%", "50% - 10%");
//   duplicatePanel->getRenderer()->setBackgroundColor(tgui::Color(40, 40, 40));
//   dbackground->add(duplicatePanel, "Panel");
//   // Title
//   std::string oldName = project.projectName;
//   auto dtitle = tgui::Label::create("Renommer le projet: \n " + oldName);
//   dtitle->getRenderer()->setTextColor(tgui::Color::White);
//   dtitle->setPosition("25%", "5%");
//   dtitle->setTextSize(24);
//   duplicatePanel->add(dtitle);
//   // Rename Box
//   auto dname = tgui::EditBox::create();
//   dname->setSize("80%", "12%");
//   dname->setPosition("10%", "35%");
//   dname->setDefaultText("Choisissez un nouveau nom ");
//   dname->getRenderer()->setRoundedBorderRadius(8);
//   dname->getRenderer()->setBackgroundColor(tgui::Color(28, 28, 36));
//   dname->getRenderer()->setBorderColor(sf::Color(55, 55, 70));
//   dname->getRenderer()->setBackgroundColorFocused(tgui::Color::White);
//   duplicatePanel->add(dname);
//   // Valid Button
//   auto dvalid = tgui::Button::create("Valider");
//   dvalid->setSize("40%", "12%");
//   dvalid->setPosition("30%", "75%");
//   dvalid->getRenderer()->setBackgroundColor(sf::Color(99, 102, 241));
//   dvalid->getRenderer()->setBackgroundColorHover(sf::Color(118, 120, 255));
//   dvalid->getRenderer()->setTextColor(sf::Color::White);
//   dvalid->getRenderer()->setBorders(0);
//   dvalid->getRenderer()->setRoundedBorderRadius(8);
//   duplicatePanel->add(dvalid);
//   // Exit Button
//   auto exitB = tgui::Button::create("x");
//   exitB->setSize("10%", "10%");
//   exitB->setPosition("0%", "0%");
//   exitB->getRenderer()->setBackgroundColor(sf::Color::Red);
//   exitB->getRenderer()->setBackgroundColorHover(sf::Color::White);
//   exitB->getRenderer()->setTextColor(sf::Color::Black);
//   exitB->getRenderer()->setBorders(0);
//   duplicatePanel->add(exitB);
//   exitB->onPress([this, dbackground]() { exitAction(dbackground); });
//
//   dvalid->onPress([this, dname, project, dbackground]() {
//     std::cout << "Renaming project id: " << project.projectId << std::endl;
//     std::string newName = static_cast<std::string>(dname->getText());
//     if (!newName.empty()) {
//       manager.renameProject(project.projectId, newName);
//       exitAction(dbackground);
//
//     } else
//       (boxError(dname));
//   });
// }

void Window::popupCreate(tgui::Panel::Ptr background) {
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
  auto exitB = tgui::Button::create("x");
  exitB->setSize("10%", "10%");
  exitB->setPosition("0%", "0%");
  exitB->getRenderer()->setBackgroundColor(sf::Color::Red);
  exitB->getRenderer()->setBackgroundColorHover(sf::Color::White);
  exitB->getRenderer()->setTextColor(sf::Color::Black);
  exitB->getRenderer()->setBorders(0);
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
                       name->getText(), 0, this->mainWindow, this->gui);
  });
}

bool Window::boxError(tgui::EditBox::Ptr box) {
  box->getRenderer()->setBorderColor(sf::Color::Red);
  box->getRenderer()->setDefaultTextColor(sf::Color::Red);
  box->setText("");
  box->setDefaultText("Changer la donnée");
  return false;
}
bool Window::checkInput(tgui::EditBox::Ptr scale, tgui::EditBox::Ptr sizePx,
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

void Window::createProj(tgui::String scale, tgui::String sizeX,
                        tgui::String sizeY, tgui::String name, unsigned int id,
                        sf::RenderWindow &window, tgui::Gui &gui) {
  // NOTE: Need verification if data isn't empty
  // NOTE: Check with server (chinsou tomas)
  unsigned int scaleInt = scale.toUInt();
  sf::Vector2u size = {sizeX.toUInt(), sizeY.toUInt()};
  std::string nameS = name.toStdString();
  // Notify the server that a Proj is being created by using
  // ClientNetworkManager
  manager.createProject(nameS, size, scaleInt);
  gui.removeAllWidgets();

  this->project =
      std::make_unique<Project>(scaleInt, size, nameS, id, mainWindow, gui);

  projectList.push_back(getProjectData(project));
  setState(projectState::GAME);
}

ProjectData Window::askProjectData() {
  initInputWidget(focusPopup::CREATE);
  return ProjectData{};
}

ProjectData Window::getProjectData(std::unique_ptr<Project> &newProj) {
  return ProjectData{newProj->getId(), 0, newProj->getName()};
}

void Window::updateCreatedProjectId(uint32_t projId) {
  projectList.back().projectId = projId;
}
