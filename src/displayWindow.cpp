#include "displayWindow.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

void Window::initWidget() {
  updateTextSize();
  gui.onViewChange([this] { updateTextSize(); });

  auto editBoxUsername = tgui::EditBox::create();
  editBoxUsername->setPosition({"37.5%", "25%"});
  editBoxUsername->setSize({"25%", "6.25%"});
  editBoxUsername->setDefaultText("Username...");
  gui.add(editBoxUsername, "Username");

  auto editBoxPassword = tgui::EditBox::copy(editBoxUsername);
  editBoxUsername->setPosition({"37.5%", "43,75%"});
  editBoxUsername->setDefaultText("Password...");
  editBoxUsername->setPasswordCharacter('*');
  gui.add(editBoxPassword, "Password");

  auto loginButton = tgui::Button::create("Login");
  loginButton->setPosition({"25%", "53.125%"});
  loginButton->setSize({"12.5%", "3.125%"});
  gui.add(loginButton);

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
                 "SFML works!",
                 sf::State::Fullscreen),
      gui(mainWindow) {
  initWidget();
}
void Window::run() {
  while (mainWindow.isOpen()) {
    processEvents();
    mainWindow.clear(sf::Color::White);
    gui.draw();
    mainWindow.display();
  }
}

// int main() {
//   Window window;
//   window.run();
//   return 0;
// }
