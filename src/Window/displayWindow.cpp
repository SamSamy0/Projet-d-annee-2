#include "displayWindow.hpp"
#include "../project/project.hpp"
#include "../project/tool.hpp"
#include "../server/clientnetwork.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Text.hpp>
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
      // Change from Login menu -> Game menu
      // state = projectState::MENU;
    }
    // initMenuWidget();
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

    //Dans un projet
    if(state == projectState::GAME && project){

      project->getMap()->detectMovement();

      
      
      
    if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      if (keyPressed->code == sf::Keyboard::Key::LShift){project->getToolBar().selectTool(PIXELSHIFT); std::cout<<"key pressed"<<std::endl;}}

      //TOOLS
      if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()){
        sf::Vector2i mousePos = mousePressed->position; // mouse position
        sf::Vector2f pos = mainWindow.mapPixelToCoords(mousePos, project->getView());
        sf::Vector2i mapPos(static_cast<int>(pos.x), static_cast<int>(pos.y));
        project->getToolBar().getSelectedTool()->onPress(mapPos);

      }

      if (auto mouseEvent = event->getIf<sf::Event::MouseButtonReleased>()){
        if (mouseEvent->button == sf::Mouse::Button::Left){
        project->getToolBar().getSelectedTool()->onRelease();
        }}

      if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
        sf::Vector2i mousePos = mouseMoved->position; // mouse position
        sf::Vector2f pos = mainWindow.mapPixelToCoords(mousePos, project->getView());
        sf::Vector2i mapPos(static_cast<int>(pos.x), static_cast<int>(pos.y));
        project->getToolBar().getSelectedTool()->onDrag(mapPos);
      }



      //SET SIZE
      if (const auto* wheelEvent = event->getIf<sf::Event::MouseWheelScrolled>()){
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LSystem )|| sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)){
          // toolType tool = project->getToolBar().getSelected();
          // if(tool == PIXELBRUSH || tool == SPRITEBRUSH){
          //   sf::Vector2u size = project->getToolBar().getSelectedTool()->getSize();
          //   float sizex = size.x;
          //   float sizey = size.y;
          //   sf::Vector2u mapSize = project->getMap()->getSize();
          //   float step = std::min(mapSize.x,mapSize.y)/100.0f;
          //
          //   if (wheelEvent->delta > 0){sizex += 1; sizey += 1;}
          //   else if(wheelEvent->delta <0){sizex -= 1 ; sizey -= 1;}
          //
          //
          //   if (sizex<1){sizex = 1;}
          //   else if(sizex > mapSize.x ){sizex = mapSize.x;}
          //   if (sizey<1){sizey = 1;}
          //   else if(sizey > mapSize.y ){sizey = mapSize.y;}
          //
          //   size =  sf::Vector2u(static_cast<unsigned int>(std::round(sizex)), static_cast<unsigned int>(std::round(sizey)));
          //   project->getToolBar().getSelectedTool()->setSize(size.x,size.y);
          // }
        }
        else{
        project->getMap()->zooming(wheelEvent); //ZOOM
        }
      }
      }

  }
}


void Window::initMenuWidget() {
  gui.removeAllWidgets();
  auto title = tgui::Label::create("Liste de projets");
  title->setPosition("2%", "5%");
  title->setTextSize(40);

  title->getRenderer()->setTextStyle(tgui::TextStyle::Underlined);
  title->getRenderer()->setTextColor(sf::Color::Black);
  gui.add(title);

  auto createProjB = tgui::Button::create("Créer Projet");
  createProjB->setPosition({"62.5%", "31.25%"});
  createProjB->setSize({"20%", "4%"});
  gui.add(createProjB);
  createProjB->onPress(&Window::createProj, this);

  //  signInButton->onPress(&AuthWindow::signIn, this, editBoxUsername,
  // editBoxPassword);

  // loginButton->onPress(&Window::login, this, editBoxUsername,
  // editBoxPassword);

  auto joinProjB = tgui::Button::create("Rejoindre Projet");
  joinProjB->setPosition({"62.5%", "18.75%"});
  joinProjB->setSize({"20%", "4%"});
  gui.add(joinProjB);
}
void Window::createProj() {
  // Notify the server that a Proj is being created by using
  // ClientNetworkManager
  // manager.createProject("Owner", 100.0, 1);
  // Vector2u = map size
  // Project newProj(1, sf::Vector2u(500, 500), mainWindow);
  gui.removeAllWidgets();
  this->project = std::make_unique<Project>(1, sf::Vector2u(500, 500), "project", 1, mainWindow, gui);
  setState(projectState::GAME);
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
// Window::Window(ClientNetworkManager &manager)
//     : mainWindow(sf::VideoMode::getDesktopMode(), "Game name",
//                  sf::State::Fullscreen),
//       gui{mainWindow}, manager{manager} {
//   initWidget();
// }
void Window::run() {
  while (mainWindow.isOpen()) {
    processEvents();
    mainWindow.clear(sf::Color::White);

    if (Window::state == projectState::LOGIN ||
      Window::state == projectState::MENU) {
    }
    else if (Window::state == projectState::GAME && project) {
      // Display Game
      project->display();
    }
    gui.draw();
    mainWindow.display();
  }
}
