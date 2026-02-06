#include "displayWindow.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

void Window::signIn(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd) {
  bool newUsr = true;
  for (int i = 0; i < DB.size(); i++) {
    if (DB[i][0] == "bonjour") {
    }
  }
}
void Window::login(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd) {
  /* Envoyer l'identifiant et le password à la base de donnée.
   * On vérifie que la connexion est correct -> autre fonction
   *
   * Tant qu'on est pas connecté, on reste sur la page de connexion
   * et un message est affiché
   */
  while (Window::checkLogin()) {
    // On affiche en boucle le menu de connexion
    // Window::menuWindow();
    usrname->setDefaultText("Wrong identifier(s)");
    gui.draw();
  }
  /* Si on est connecté, le menu est affiché avec les projets.
   * */
  Window::menuWindow();
}

void Window::initWidget() {
  updateTextSize();
  gui.onViewChange([this] { updateTextSize(); });

  auto editBoxUsername = tgui::EditBox::create();
  editBoxUsername->setPosition({"37.5%", "25%"});
  editBoxUsername->setSize({"25%", "6.25%"});
  editBoxUsername->setDefaultText("Username...");
  gui.add(editBoxUsername, "Username");

  auto editBoxPassword = tgui::EditBox::copy(editBoxUsername);
  editBoxPassword->setPosition({"37.5%", "43,75%"});
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
    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      if (keyPressed->code == sf::Keyboard::Key::Escape)
        mainWindow.close();
    }
  }
}

Window::Window()
    : mainWindow(sf::VideoMode::getDesktopMode(),
                 "Game name",
                 sf::State::Fullscreen),
      gui(mainWindow) {
  initWidget();
}
void Window::run() {
  while (mainWindow.isOpen()) {
    processEvents();
    mainWindow.clear(sf::Color::White);
    mainWindow.display();
  }
}
