#include "Menu.hpp"
#include "Identifier.hpp"
#include "displayWindow.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Text.hpp>
#include <TGUI/Widgets/TextArea.hpp>
void MenuWin::processEvents() {
  while (const std::optional event = menuWindow.pollEvent()) {
    MenuGui.handleEvent(*event);
    // Close the Window
    if (event->is<sf::Event::Closed>())
      menuWindow.close();

    // Echap to escape
    if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      if (keyPressed->code == sf::Keyboard::Key::Escape)
        menuWindow.close();
    }
  }
}
void MenuWin::updateTextSize() {
  const float windowHeight = MenuGui.getView().getRect().height;
  MenuGui.setTextSize(static_cast<unsigned int>(0.03 * windowHeight));
}

void MenuWin::menuWidget() {
  updateTextSize();
  MenuGui.onViewChange([this] { updateTextSize(); });

  // auto editBoxUsername = tgui::EditBox::create();
  // editBoxUsername->setPosition({"37.5%", "25%"});
  // editBoxUsername->setSize({"25%", "6.25%"});
  // editBoxUsername->setDefaultText("Username...");
  // MenuGui.add(editBoxUsername, "Username");
  //
  // auto editBoxPassword = tgui::EditBox::copy(editBoxUsername);
  // editBoxPassword->setPosition({"37.5%", "43.75%"});
  // editBoxPassword->setDefaultText("Password...");
  // editBoxPassword->setPasswordCharacter('*');
  // MenuGui.add(editBoxPassword, "Password");

  auto createProjB = tgui::Button::create("Créer Projet");
  createProjB->setPosition({"62.5%", "31.25%"});
  createProjB->setSize({"12.5%", "3.125%"});
  MenuGui.add(createProjB);

  // loginButton->onPress(&Window::login, this, editBoxUsername,
  // editBoxPassword);

  auto joinProjB = tgui::Button::create("Rejoindre Projet");
  joinProjB->setPosition({"62.5%", "18.75%"});
  joinProjB->setSize({"12.5%", "3.125%"});
  MenuGui.add(joinProjB);
  // signInButton->onPress(&MenuWin::signIn, this, editBoxUsername,
  //                       editBoxPassword);
}
MenuWin::MenuWin()
    : menuWindow(sf::VideoMode::getDesktopMode(), "Menu name",
                 sf::State::Fullscreen),
      MenuGui(menuWindow) {
  menuWidget();
}
void MenuWin::run() {
  while (menuWindow.isOpen()) {
    processEvents();
    menuWindow.clear(sf::Color::White);
    MenuGui.draw();
    menuWindow.display();
  }
}
