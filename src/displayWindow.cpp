#include "displayWindow.hpp"
#include "Identifier.hpp"
#include "server/clientnetwork.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Text.hpp>
#include <TGUI/Widgets/TextArea.hpp>

void AuthWindow::signIn(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd) {
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
      this->state = projectState::MENU;
      initMenuWidget();
    }
  }
}

void AuthWindow::login(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd) {
  manager.login(static_cast<std::string>(usrname->getText()),
                static_cast<std::string>(pswd->getText()));
  // If the handler changed isLoggedIn to true, access granted
  if (isLoggedIn) {
    state = projectState::MENU;
    initMenuWidget();
  }
  // The identifiers were wrong
  else {
    usrname->setDefaultText("Wrong Identifier(s)");
    pswd->setText("");
  }
}
void AuthWindow::loginWidget() {
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

  // loginButton->onPress(&Window::login, this, editBoxUsername,
  // editBoxPassword);

  auto signInButton = tgui::Button::create("Sign in");
  signInButton->setPosition({"62.5%", "53.125%"});
  signInButton->setSize({"12.5%", "3.125%"});
  gui.add(signInButton);
  signInButton->onPress(&AuthWindow::signIn, this, editBoxUsername,
                        editBoxPassword);
}

// Right now, I am putting them in Menu.cpp
//  void AuthWindow::menuWidget() {
//  Here will be the menu's widgets
// }

void AuthWindow::initWidget() {
  updateTextSize();
  gui.onViewChange([this] { updateTextSize(); });

  if (this->state == projectState::LOGIN) {
    AuthWindow::loginWidget();
  } else if (this->state == projectState::MENU) {
    // AuthWindow::menuWidget();
  }
  // else if (this->state == projectState::GAME){
  // Display the map if we enter a project.
  //
  // }
}
void AuthWindow::updateTextSize() {
  const float windowHeight = gui.getView().getRect().height;
  gui.setTextSize(static_cast<unsigned int>(0.03 * windowHeight));
}
void AuthWindow::processEvents() {
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
  }
}

void AuthWindow::initMenuWidget() {
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

  // loginButton->onPress(&Window::login, this, editBoxUsername,
  // editBoxPassword);

  auto joinProjB = tgui::Button::create("Rejoindre Projet");
  joinProjB->setPosition({"62.5%", "18.75%"});
  joinProjB->setSize({"20%", "4%"});
  gui.add(joinProjB);
}

AuthWindow::AuthWindow(ClientNetworkManager &manager)
    : mainWindow(sf::VideoMode::getDesktopMode(), "Game name",
                 sf::State::Fullscreen),
      gui{mainWindow}, manager{manager} {
  initWidget();
}
void AuthWindow::run() {
  while (mainWindow.isOpen()) {
    processEvents();
    mainWindow.clear(sf::Color::White);

    if (AuthWindow::state == projectState::LOGIN ||
        AuthWindow::state == projectState::MENU) {
      gui.draw();

    } else if (AuthWindow::state == projectState::GAME) {
      // Display Game
    }
    mainWindow.display();
  }
}
