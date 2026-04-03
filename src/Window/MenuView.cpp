#include "MenuView.hpp"
#include "Application.hpp"
#include "projectWidgets/GameView.hpp"

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

  if (shareToken != "FFFFF") {
    rightPanel->add(displayToken());
  }
}
void MenuView::clearProjList() { projectList.clear(); }
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
  if (project.role >= 1) {
    // On désactive les deux derniers items (index 3 et 4)
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
  // menu->onUnfocus([this, menu]() { gui.remove(menu); });
  menu->onItemSelect([this, menu, id, toHover, project, projectRole, &manager,
                      &gui](const tgui::String &item) {
    // Delete Action
    if (item == "Supprimer") {
      deleteProject(id);
      // Open Action
    } else if (item == "Ouvrir") {
      std::cout << "Ouverture du projet " << std::endl;
      std::cout << "futur role : " << static_cast<int>(project.role)
                << std::endl;

      manager.getProjectData(id);

      // Rename Action
    } else if (item == "Renommer") {
      std::cout << "appuyé" << std::endl;
      // NOTE: Sending project reference to server with "project"
      initInputWidget(focusPopup::RENAME, project);
    } else if (item == "Dupliquer") {
      initInputWidget(focusPopup::DUPLICATE, project);

    } else if (item == "Partager") {
      initInputWidget(focusPopup::TOKEN, project);
    }
    //                else if (item == "Quitter") {
    // // If owner
    // if (projectRole == 2) {
    //   // display choseSuccesorWindow
    // }
    // }
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
    std::cout << "Suppression du projet " << id << std::endl;
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
  exitB->setSize("10%", "10%");
  exitB->setPosition("0%", "0%");
  exitB->getRenderer()->setBackgroundColor(sf::Color(102, 178, 255));
  exitB->getRenderer()->setBackgroundColorHover(sf::Color::White);
  exitB->getRenderer()->setTextColor(sf::Color::Black);
  exitB->getRenderer()->setBorders(0);
  renamePanel->add(exitB);
  exitB->onPress([this, renameBackground]() { exitAction(renameBackground); });

  rvalid->onPress([this, rname, project, renameBackground, view, &gui,
                   &manager]() {
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
        std::cout << "duplicating projects id: " << project.projectId
                  << std::endl;
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
  std::cout << "token in app" << newToken << std::endl;
  shareToken = newToken;
  std::cout << "token changed " << shareToken << std::endl;
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
  comboBox->addItem("Editeur");
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
    std::cout << "Element selectionné : " << selected << std::endl;

    if (selected == "Editeur") {
      std::cout << "Editeur" << std::endl;
      generateToken(1, project.projectId);

    } else if (selected == "Spectateur") {
      std::cout << "Spectateur" << std::endl;
      generateToken(0, project.projectId);
    }
    exitAction(background);
  });
}
