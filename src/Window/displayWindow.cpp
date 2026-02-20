#include "displayWindow.hpp"
#include "../project/project.hpp"
#include "../project/tool.hpp"
#include "../server/clientnetwork.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Layout.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Text.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/ListBox.hpp>
#include <TGUI/Widgets/ScrollablePanel.hpp>
#include <TGUI/Widgets/Scrollbar.hpp>
#include <TGUI/Widgets/TextArea.hpp>
#include <iostream>
#include <memory>
#include <vector>

void Window::signIn(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd) {
  // NOTE: I have to ask how the usernames are stored in the server
  // -> Asked Tomas
  // NOTE: I have to put the condition username 3-15 car

  // Sending Identifiers to server
  // NOTE: Need to forbid empty usrnames
  if (usrname->getText() != "") {
    manager.askRegister(static_cast<std::string>(usrname->getText()),
                        static_cast<std::string>(pswd->getText()));
    std::cout << usrname->getText() << "   " << pswd->getText() << std::endl;

    if (!isLoggedIn) {
      // Ask another Usrname
      usrname->setText("");
      usrname->setDefaultText("Username already taken or invalid ");
      pswd->setText("");
    } else {
      // Access granted
      // setState(projectState::MENU);
    }
    setState(projectState::MENU);
  }
}

void Window::login(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd) {
  manager.login(static_cast<std::string>(usrname->getText()),
                static_cast<std::string>(pswd->getText()));
  // If the handler changed isLoggedIn to true, access granted
  if (isLoggedIn) {
    setState(projectState::MENU);
  }
  // The identifiers were wrong
  else {
    usrname->setText("");
    usrname->setDefaultText("Wrong Identifier(s)");
    pswd->setText("");
  }
}
void Window::loginWidget() {
  auto back = tgui::Panel::create();
  back->setSize("32%", "42%");
  back->setPosition("34%", "29%");
  back->getRenderer()->setBackgroundColor(tgui::Color(28, 28, 36));
  back->getRenderer()->setRoundedBorderRadius(12);
  back->getRenderer()->setBorderColor(sf::Color(55, 55, 70));
  gui.add(back, "Main login");

  auto title = tgui::Label::create("Bienvenue");
  title->setPosition("32%", "8%");
  title->setTextSize(28);
  title->getRenderer()->setTextColor(sf::Color(240, 240, 255));
  back->add(title);

  auto editBoxUsername = tgui::EditBox::create();
  editBoxUsername->setPosition({"10%", "32%"});
  editBoxUsername->setSize({"80%", "13%"});
  editBoxUsername->setDefaultText("Pseudonyme...");
  editBoxUsername->getRenderer()->setBackgroundColor(sf::Color(20, 20, 28));
  editBoxUsername->getRenderer()->setBackgroundColorHover(
      sf::Color(24, 24, 34));
  editBoxUsername->getRenderer()->setTextColor(sf::Color::White);
  editBoxUsername->getRenderer()->setDefaultTextColor(sf::Color(90, 90, 115));
  editBoxUsername->getRenderer()->setBorders(1);
  editBoxUsername->getRenderer()->setBorderColor(sf::Color(55, 55, 75));
  editBoxUsername->getRenderer()->setRoundedBorderRadius(6);
  // gui.add(editBoxUsername, "Username");
  back->add(editBoxUsername, "Username");

  auto editBoxPassword = tgui::EditBox::copy(editBoxUsername);
  editBoxPassword->getRenderer()->setTextColor(tgui::Color(255, 255, 255));
  editBoxPassword->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  editBoxPassword->setPosition({"10%", "50%"});
  editBoxPassword->setDefaultText("Mot de Passe...");
  editBoxPassword->setPasswordCharacter('*');
  // gui.add(editBoxPassword, "Password");
  back->add(editBoxPassword);

  auto loginButton = tgui::Button::create("Connexion");
  loginButton->getRenderer()->setBackgroundColor(sf::Color(99, 102, 241));
  loginButton->getRenderer()->setBackgroundColorHover(sf::Color(118, 120, 255));
  loginButton->getRenderer()->setTextColor(sf::Color::White);
  loginButton->getRenderer()->setBorders(0);
  loginButton->getRenderer()->setRoundedBorderRadius(8);
  loginButton->setPosition({"53%", "72%"});
  loginButton->setSize({"37%", "13%"});
  back->add(loginButton);
  // gui.add(loginButton);
  loginButton->onPress(&Window::login, this, editBoxUsername, editBoxPassword);

  auto signInButton = tgui::Button::create("Inscription");
  signInButton->getRenderer()->setTextColor(tgui::Color(170, 170, 200));
  signInButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  signInButton->getRenderer()->setBackgroundColorHover(sf::Color(45, 45, 60));
  signInButton->getRenderer()->setBorders(1);
  signInButton->getRenderer()->setBorderColor(sf::Color(70, 70, 95));
  signInButton->setPosition("10%", "72%");
  signInButton->setSize("37%", "13%");
  back->add(signInButton);
  // gui.add(signInButton);
  signInButton->onPress(&Window::signIn, this, editBoxUsername,
                        editBoxPassword);
}

void Window::initWidget() {
  gui.removeAllWidgets();
  updateTextSize();
  gui.onViewChange([this] { updateTextSize(); });

  if (this->state == projectState::LOGIN) {
    Window::loginWidget();
  } else if (this->state == projectState::MENU) {
    Window::initMenuWidget();
  } else if (this->state == projectState::GAME) {
    // Ajout du bouton de Home (retour en arrière)
    auto homeButton = tgui::Button::create();
    homeButton->setSize(30, 30);
    homeButton->setPosition(15, 5);
    homeButton->getRenderer()->setTexture("../res/images/accueil.png");
    homeButton->getRenderer()->setBorders({0});
    homeButton->onPress(&Window::setState, this, projectState::MENU);
    gui.add(homeButton);
  }
}
void Window::updateTextSize() {
  const float windowHeight = gui.getView().getRect().height;
  gui.setTextSize(static_cast<unsigned int>(0.03 * windowHeight));
}
void Window::processEvents() {
  while (const std::optional event = mainWindow.pollEvent()) {
    gui.handleEvent(*event);
    // Close the Window
    if (event->is<sf::Event::Closed>())
      mainWindow.close();

    // Echap to escape
    if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      if (keyPressed->code == sf::Keyboard::Key::Escape)
        mainWindow.close();
    }
    if (const auto *mouseClick =
            event->getIf<sf::Event::MouseButtonPressed>()) {
      auto popup = gui.get("popup");
      auto background = gui.get("background");
      // If popup exists
      if (popup) {
        // Gets position of where menu pops
        sf::Vector2f clickPos(mouseClick->position.x, mouseClick->position.y);
        // If click outside the menu, then remove it
        if (!popup->isMouseOnWidget(clickPos)) {
          gui.remove(popup);
        }
      }
      if (background) {
        // Cast to Panel to use (->get("dataPanel"))
        auto backgroundPanel =
            std::dynamic_pointer_cast<tgui::Panel>(background);
        auto dataPanel = backgroundPanel->get("dataPanel");
        sf::Vector2f clickPos(mouseClick->position.x, mouseClick->position.y);
        if (dataPanel && !dataPanel->isMouseOnWidget(clickPos)) {
          gui.remove(background);
        }
      }
    }
    if (event->getIf<sf::Event::MouseWheelScrolled>()) {
      auto popup = gui.get("popup");
      if (popup) {
        gui.remove(popup);
      }
    }

    // Dans un projet
    if (state == projectState::GAME && project) {
      project->getMap()->detectZooming(*event); // ZOOM
      //
      if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        leftClickEvent();
      }
      if (auto mouseEvent = event->getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseEvent->button == sf::Mouse::Button::Left) {
          project->getToolBar().getSelectedTool()->stopDrawing();
        }
      }
    }
  }
}
void Window::leftClickEvent() {
  toolType tool = project->getToolBar().getSelected();
  sf::Vector2i mousePos = sf::Mouse::getPosition(mainWindow); // mouse position
  sf::Vector2f pos = mainWindow.mapPixelToCoords(mousePos, project->getView());
  sf::Vector2i mapPos(static_cast<int>(pos.x), static_cast<int>(pos.y));

  switch (tool) {
  case PIXELBRUSH: {
    if (project->getMap()->isInside(mapPos) &&
        !gui.getWidgetAtPos(sf::Vector2f(mousePos), true)) {
      project->getToolBar().getSelectedTool()->drawOn(mapPos);
    }
    break;
  }
  case PIXELSHIFT: {
    break;
  }

  case SPRITEBRUSH: {
    break;
  }
  }
}

void Window::initMenuWidget() {
  gui.removeAllWidgets();

  auto leftPanel = tgui::Panel::create();
  leftPanel->setSize("55%", "100%");
  leftPanel->getRenderer()->setBackgroundColor(sf::Color(22, 22, 28));
  gui.add(leftPanel);

  auto title = tgui::Label::create("Mes Projets");
  title->setPosition("5%", "6%");
  title->setTextSize(28);
  title->getRenderer()->setTextColor(sf::Color(255, 255, 255));
  leftPanel->add(title);

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
    btn->onPress(&Window::showProjectMenu, this, projectList[i].projectId);
    row->add(btn);
    btn->setTextSize(20);
  }
}
void Window::showProjectMenu(long long id) {
  auto menu = tgui::ListBox::create();
  menu->addItem("Ouvrir");
  menu->addItem("Supprimer");
  menu->setSize(150, 90);
  menu->setItemHeight(45);

  sf::Vector2i mousePos = gui.getLastMousePosition();
  menu->setPosition(mousePos.x + 2, mousePos.y + 2);

  gui.add(menu, "popup");
  menu->setTextSize(20);
  // menu->onUnfocus([this, menu]() { gui.remove(menu); });
  menu->onItemSelect([this, menu, id](const tgui::String &item) {
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
  });
}
void Window::createProj(tgui::String scale, tgui::String sizeX,
                        tgui::String sizeY, tgui::String name, unsigned int id,
                        sf::RenderWindow &window, tgui::Gui &gui) {
  // NOTE: Need verification if data isn't empty
  unsigned int scaleInt = scale.toUInt();
  sf::Vector2u size = {sizeX.toUInt(), sizeY.toUInt()};
  std::string nameS = name.toStdString();
  // Notify the server that a Proj is being created by using
  // ClientNetworkManager
  manager.createProject("Owner", size, scaleInt);
  gui.removeAllWidgets();

  this->project =
      std::make_unique<Project>(scaleInt, size, nameS, id, mainWindow, gui);

  projectList.push_back(getProjectData(project));
  setState(projectState::GAME);
}

ProjectData Window::getProjectData(std::unique_ptr<Project> &newProj) {
  return ProjectData{newProj->getId(), 0, newProj->getName()};
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
  data->getRenderer()->setBackgroundColor(tgui::Color::White);
  background->add(data, "dataPanel");

  auto title = tgui::Label::create("Nouveau projet");
  title->setPosition("37%", "5%");
  title->setTextSize(40);
  title->getRenderer()->setTextStyle(tgui::TextStyle::Underlined);
  title->getRenderer()->setTextColor(sf::Color::Black);
  data->add(title);

  auto name = tgui::EditBox::create();
  name->setSize("80%", "12%");
  name->setPosition("10%", "20%");
  name->setDefaultText("Nom: ...");
  data->add(name);

  auto scale = tgui::EditBox::create();
  scale->setInputValidator("[0-9]*");
  scale->setMaximumCharacters(5);
  scale->setSize("80%", "12%");
  scale->setPosition("10%", "42%");
  scale->setDefaultText("Echelle: ...");
  data->add(scale);

  auto sizePx = tgui::EditBox::create();
  sizePx->setInputValidator("[0-9]*");
  sizePx->setMaximumCharacters(5);
  sizePx->setSize("80%", "12%");
  sizePx->setPosition("10%", "64%");
  sizePx->setDefaultText("Taille (x): ...");
  data->add(sizePx);

  auto sizePy = tgui::EditBox::create();
  sizePy->setInputValidator("[0-9]*");
  sizePy->setMaximumCharacters(5);
  sizePy->setSize("40%", "12%");
  sizePy->setPosition("50%", "64%");
  sizePy->setDefaultText("Taille (y): ...");
  data->add(sizePy);

  auto valid = tgui::Button::create("Valider");
  valid->setSize("40%", "12%");
  valid->setPosition("30%", "90% - 20");
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

ProjectData Window::askProjectData() {
  initDataWidget();
  // ProjectData newProj = initDataWidget();
  // projectList.push_back(newProj);
  return ProjectData{};
}
void Window::setState(projectState newState) {
  this->state = newState;
  this->initWidget();
}

Window::Window(ClientNetworkManager &manager)
    : mainWindow(sf::VideoMode::getDesktopMode(), "Game name",
                 sf::State::Fullscreen),
      gui{mainWindow}, manager{manager}, project{nullptr} {
  initWidget();
}

void Window::setProjectList(std::vector<ProjectData> newlist) {
  projectList = newlist;
}
void Window::run() {
  while (mainWindow.isOpen()) {
    processEvents();
    mainWindow.clear(sf::Color(35, 35, 40));

    if (Window::state == projectState::LOGIN ||
        Window::state == projectState::MENU) {
      gui.draw();
    } else if (Window::state == projectState::GAME && project) {
      // Display Game
      project->display();
    }
    gui.draw();
    mainWindow.display();
  }
}
