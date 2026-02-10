#include "displayWindow.hpp"
#include "Identifier.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Text.hpp>
#include <TGUI/Widgets/TextArea.hpp>

bool AuthWindow::signIn(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd) {
  // NOTE: I have to put the condition username 3-15 car

  for (auto i = DB.begin(); i != DB.end(); i++) {
    if (i->getusername() == usrname->getText()) {
      // Demander un autre Pseudo
      usrname->setText("");

      usrname->setDefaultText("Username already taken or invalid ");
      pswd->setText("");
      return false;
    }
  }
  // New user
  // WARNING: Je dois attendre le "Network Manager" de Tomas et Chinsou
  if (usrname->getText() != "") {
    Identifier id = Identifier{static_cast<std::string>(usrname->getText()),
                               static_cast<std::string>(pswd->getText())};
    DB.push_back({id});
    // NOTE: C'est ici que je devrai envoyer au serveur les identifiants de
    // l'utilisateur
    std::cout << id.getusername() << "   " << pswd->getText() << std::endl;
    // NOTE: Change mode LOGGIN -> MENU for changing what gui draws
  }
  return true;
}
void AuthWindow::login(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd) {
  /* Envoyer l'identifiant et le password à la base de donnée.
   * On vérifie que la connexion est correct -> autre fonction
   *
   * Tant qu'on est pas connecté, on reste sur la page de connexion
   * et un message est affiché
   */
  // while (!Window::checkLogin()) {
  //   // On affiche en boucle le menu de connexion
  //   // Window::menuWindow();
  //   usrname->setDefaultText("Wrong identifier(s)");
  //   gui.draw();
  // }
  /* Si on est connecté, le menu est affiché avec les projets.
   * */
  // Window::menuWindow();
}
void AuthWindow::loginWidget() {
  auto editBoxUsername = tgui::EditBox::create();
  editBoxUsername->setPosition({"37.5%", "25%"});
  editBoxUsername->setSize({"25%", "6.25%"});
  editBoxUsername->setDefaultText("Username...");
  AuthGui.add(editBoxUsername, "Username");

  auto editBoxPassword = tgui::EditBox::copy(editBoxUsername);
  editBoxPassword->setPosition({"37.5%", "43.75%"});
  editBoxPassword->setDefaultText("Password...");
  editBoxPassword->setPasswordCharacter('*');
  AuthGui.add(editBoxPassword, "Password");

  auto loginButton = tgui::Button::create("Login");
  loginButton->setPosition({"25%", "53.125%"});
  loginButton->setSize({"12.5%", "3.125%"});
  AuthGui.add(loginButton);

  // loginButton->onPress(&Window::login, this, editBoxUsername,
  // editBoxPassword);

  auto signInButton = tgui::Button::create("Sign in");
  signInButton->setPosition({"62.5%", "53.125%"});
  signInButton->setSize({"12.5%", "3.125%"});
  AuthGui.add(signInButton);
  signInButton->onPress(&AuthWindow::signIn, this, editBoxUsername,
                        editBoxPassword);
}

// Right now, I am putting them in Menu.cpp
//  void AuthWindow::menuWidget() {
//  Here will be the menu's widgets
// }

void AuthWindow::initWidget() {
  updateTextSize();
  AuthGui.onViewChange([this] { updateTextSize(); });

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
  const float windowHeight = AuthGui.getView().getRect().height;
  AuthGui.setTextSize(static_cast<unsigned int>(0.03 * windowHeight));
}
void AuthWindow::processEvents() {
  while (const std::optional event = mainWindow.pollEvent()) {
    AuthGui.handleEvent(*event);
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

AuthWindow::AuthWindow()
    : mainWindow(sf::VideoMode::getDesktopMode(), "Game name",
                 sf::State::Fullscreen),
      AuthGui(mainWindow) {
  initWidget();
}
void AuthWindow::run() {
  while (mainWindow.isOpen()) {
    processEvents();
    mainWindow.clear(sf::Color::White);
    AuthGui.draw();
    mainWindow.display();
  }
}
