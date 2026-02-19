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
  auto editBoxUsername = tgui::EditBox::create();
  editBoxUsername->setPosition({"37.5%", "25%"});
  editBoxUsername->setSize({"25%", "6.25%"});
  editBoxUsername->setDefaultText("Username...");
  gui.add(editBoxUsername, "Username");

  auto editBoxPassword = tgui::EditBox::copy(editBoxUsername);
  editBoxPassword->setPosition({"37.5%", "43.75%"});
  editBoxPassword->setDefaultText("Password...");
  editBoxPassword->setPasswordCharacter('*');
  gui.add(editBoxPassword, "Password");

  auto loginButton = tgui::Button::create("Login");
  loginButton->setPosition({"25%", "53.125%"});
  loginButton->setSize({"12.5%", "3.125%"});
  gui.add(loginButton);
  loginButton->onPress(&Window::login, this, editBoxUsername, editBoxPassword);

  auto signInButton = tgui::Button::create("Sign in");
  signInButton->setPosition({"62.5%", "53.125%"});
  signInButton->setSize({"12.5%", "3.125%"});
  gui.add(signInButton);
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
  auto title = tgui::Label::create("Liste de projets");
  title->setPosition("15.25%", "5%");
  title->setTextSize(40);
  title->getRenderer()->setTextStyle(tgui::TextStyle::Underlined);
  title->getRenderer()->setTextColor(sf::Color::Black);
  gui.add(title);
  displayProjList();

  auto createProjB = tgui::Button::create("Créer Projet");
  createProjB->setPosition({"62.5%", "31.25%"});
  createProjB->setSize({"20%", "4%"});
  gui.add(createProjB);
  createProjB->onPress(&Window::askProjectData, this);

  auto joinProjB = tgui::Button::create("Rejoindre Projet");
  joinProjB->setPosition({"62.5%", "18.75%"});
  joinProjB->setSize({"20%", "4%"});
  gui.add(joinProjB);
}

void Window::displayProjList() {
  auto panel = tgui::ScrollablePanel::create();
  panel->setPosition(0, "15%");
  panel->setSize("50%", "50%");
  gui.add(panel);
  panel->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Always);

  for (int i = 0; i < projectList.size(); i++) {
    auto row = tgui::Panel::create();
    row->setSize("100%", 80);
    row->setPosition(0, i * 80);
    auto label = tgui::Label::create(projectList[i].projectName);
    label->setPosition(0, "center");
    row->add(label);

    auto btn = tgui::Button::create("...");
    btn->setSize(30, 30);
    //
    btn->setPosition("100% - 40", "center");

    btn->onPress(&Window::showProjectMenu, this, to_string(i));

    row->add(btn);

    panel->add(row);
  }
}
void Window::showProjectMenu(std::string projName) {
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
  menu->onItemSelect([this, menu, projName](const tgui::String &item) {
    if (item == "Supprimer") {
      std::cout << "Suppression du projet " << projName << std::endl;
      // manager.deleteProject(...)
    } else if (item == "Ouvrir") {
      std::cout << "Ouverture du projet " << projName << std::endl;
      setState(projectState::GAME);
    }
    gui.remove(menu);
  });
}
void Window::createProj() {
  // Notify the server that a Proj is being created by using
  // ClientNetworkManager
  manager.createProject("Owner", sf::Vector2u(500, 500), 1.0);
  gui.removeAllWidgets();
  this->project = std::make_unique<Project>(1, sf::Vector2u(500, 500),
                                            "project", 1, mainWindow, gui);

  projectList.push_back(ProjectData{});
  setState(projectState::GAME);
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
  scale->setSize("80%", "12%");
  scale->setPosition("10%", "42%");
  scale->setDefaultText("Echelle: ...");
  data->add(scale);

  auto sizePx = tgui::EditBox::create();
  sizePx->setSize("80%", "12%");
  sizePx->setPosition("10%", "64%");
  sizePx->setDefaultText("Taille (x): ...");
  data->add(sizePx);

  auto sizePy = tgui::EditBox::create();
  sizePy->setSize("40%", "12%");
  sizePy->setPosition("50%", "64%");
  sizePy->setDefaultText("Taille (y): ...");
  data->add(sizePy);

  auto valid = tgui::Button::create("Valider");
  valid->setSize("40%", "12%");
  valid->setPosition("30%", "90% - 20");
  data->add(valid);
  valid->onPress([this, background]() {
    gui.remove(background);
    createProj();
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
    mainWindow.clear(sf::Color::White);

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
