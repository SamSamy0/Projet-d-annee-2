#include "Window.hpp"

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
  rightPanel->add(joinProjB);
}

void Window::displayProjList(tgui::Panel::Ptr parent) {
  auto panel = tgui::ScrollablePanel::create();
  panel->setPosition("5%", "18%");
  panel->setSize("90%", "78%");
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
    parent->add(row);
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
    btn->onPress(&Window::showProjectMenu, this, projectList[i].projectId, btn);
    btn->setTextSize(20);
  }
}

void Window::showProjectMenu(long long id, tgui::Button::Ptr toHover) {
  activeMoreButton = toHover;
  activeMoreButton->getRenderer()->setBackgroundColor(sf::Color(55, 55, 70));
  auto menu = tgui::ListBox::create();
  menu->getScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  menu->getRenderer()->setBorders(0);
  menu->getRenderer()->setBackgroundColor(sf::Color(40, 40, 52));
  menu->getRenderer()->setTextColor(tgui::Color::White);
  menu->addItem("Ouvrir");
  menu->addItem("Supprimer");
  menu->setSize(150, 90);
  menu->setItemHeight(45);

  sf::Vector2i mousePos = gui.getLastMousePosition();
  menu->setPosition(mousePos.x + 2, mousePos.y + 2);

  gui.add(menu, "popup");
  menu->setTextSize(20);
  // menu->onUnfocus([this, menu]() { gui.remove(menu); });
  menu->onItemSelect([this, menu, id, toHover](const tgui::String &item) {
    if (item == "Supprimer") {
      auto it = std::find_if(
          projectList.begin(), projectList.end(),
          [id](const ProjectData &p) { return p.projectId == id; });
      if (it != projectList.end()) {
        projectList.erase(it);
        std::cout << "Suppression du projet " << id << std::endl;
        initMenuWidget();
      }
      // manager.deleteProject(...)
    } else if (item == "Ouvrir") {
      std::cout << "Ouverture du projet " << std::endl;
      setState(projectState::GAME);
    }
    gui.remove(menu);
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

void Window::initDataWidget() {
  // Grey Backgroung
  auto background = tgui::Panel::create();
  background->setSize("100%", "100%");
  background->getRenderer()->setBackgroundColor({40, 40, 40, 150});
  gui.add(background, "background");
  background->setTextSize(30);

  // Box for project's data
  auto data = tgui::Panel::create();
  data->setSize("35%", "40%");
  data->setPosition("50%-17.5%", "50%-20% ");
  data->getRenderer()->setBackgroundColor(tgui::Color(40, 40, 40));
  background->add(data, "dataPanel");

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

  auto valid = tgui::Button::create("Valider");
  valid->setSize("40%", "12%");
  valid->setPosition("30%", "90% - 20");
  valid->getRenderer()->setBackgroundColor(sf::Color(99, 102, 241));
  valid->getRenderer()->setBackgroundColorHover(sf::Color(118, 120, 255));
  valid->getRenderer()->setTextColor(sf::Color::White);
  valid->getRenderer()->setBorders(0);
  valid->getRenderer()->setRoundedBorderRadius(8);
  data->add(valid);
  // valid->onPress([this, background]() {
  //   gui.remove(background);
  //   createProj();
  //   // createProj(unsigned int scale, sf::Vector2u size, std::string name,
  //   //                     unsigned int id, sf::RenderWindow &window,
  //   //                     tgui::Gui &gui);
  // });
  valid->onPress([this, scale, sizePx, sizePy, name]() {
    // Les getText() sont appelés au moment du clic !
    this->createProj(scale->getText(), sizePx->getText(), sizePy->getText(),
                     name->getText(), 0, this->mainWindow, this->gui);
  });
  // BUG: Need to return values to create Project

  // return Project{scale->getText(), sf::Vector2u{sizeX, sizeY},
  // name->getText(),
  //                &this->mainWindow, &this->gui};
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
      std::make_unique<Project>(scaleInt, size, nameS, id, mainWindow, gui, manager);

  projectList.push_back(getProjectData(project));
  setState(projectState::GAME);
}

ProjectData Window::askProjectData() {
  initDataWidget();
  // ProjectData newProj = initDataWidget();
  // projectList.push_back(newProj);
  return ProjectData{};
}

ProjectData Window::getProjectData(std::unique_ptr<Project> &newProj) {
  return ProjectData{newProj->getId(), 0, newProj->getName()};
}
