#include "LoginView.hpp"
#include "Application.hpp"
#include <iostream>
LoginView::LoginView(Application &app) : View(app) {};

void LoginView::init() {
  auto &gui = app_.getGui();
  // SubWindow
  auto back = tgui::Panel::create();
  back->setSize("32%", "42%");
  back->setPosition("34%", "29%");
  back->getRenderer()->setBackgroundColor(tgui::Color(28, 28, 36));
  back->getRenderer()->setRoundedBorderRadius(12);
  back->getRenderer()->setBorderColor(sf::Color(55, 55, 70));
  gui.add(back, "Main login");

  // Title
  auto title = tgui::Label::create("Bienvenue");
  title->setPosition("32%", "8%");
  title->setTextSize(28);
  title->getRenderer()->setTextColor(sf::Color(240, 240, 255));
  back->add(title);

  // Username
  auto editBoxUsername = tgui::EditBox::create();
  editBoxUsername->setPosition({"10%", "32%"});
  editBoxUsername->setSize({"80%", "13%"});
  editBoxUsername->setDefaultText("Pseudo...");
  editBoxUsername->getRenderer()->setBackgroundColor(sf::Color(20, 20, 28));
  editBoxUsername->getRenderer()->setBackgroundColorHover(
      sf::Color(24, 24, 34));
  editBoxUsername->getRenderer()->setTextColor(sf::Color::White);
  editBoxUsername->getRenderer()->setDefaultTextColor(sf::Color(90, 90, 115));
  editBoxUsername->getRenderer()->setBorders(1);
  editBoxUsername->getRenderer()->setBorderColor(sf::Color(55, 55, 75));
  editBoxUsername->getRenderer()->setRoundedBorderRadius(6);
  back->add(editBoxUsername, "Username");

  // Password
  auto editBoxPassword = tgui::EditBox::copy(editBoxUsername);
  editBoxPassword->getRenderer()->setTextColor(tgui::Color(255, 255, 255));
  editBoxPassword->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  editBoxPassword->setPosition({"10%", "50%"});
  editBoxPassword->setDefaultText("Mot de Passe...");
  editBoxPassword->setPasswordCharacter('*');
  back->add(editBoxPassword, "Password");

  // Error label (hidden by default)
  auto errorLabel = tgui::Label::create("");
  errorLabel->setPosition({"10%", "65%"});
  errorLabel->setSize({"80%", "8%"});
  errorLabel->getRenderer()->setTextColor(sf::Color(220, 80, 80));
  errorLabel->setTextSize(13);
  back->add(errorLabel, "ErrorLabel");

  // Login Button
  auto loginButton = tgui::Button::create("Connexion");
  loginButton->getRenderer()->setBackgroundColor(sf::Color(99, 102, 241));
  loginButton->getRenderer()->setBackgroundColorHover(sf::Color(118, 120, 255));
  loginButton->getRenderer()->setTextColor(sf::Color::White);
  loginButton->getRenderer()->setBorders(0);
  loginButton->getRenderer()->setRoundedBorderRadius(8);
  loginButton->setPosition({"53%", "72%"});
  loginButton->setSize({"37%", "13%"});
  back->add(loginButton);
  loginButton->onPress(&LoginView::login, this, editBoxUsername,
                       editBoxPassword);

  // SignIn Button
  auto signInButton = tgui::Button::create("Inscription");
  signInButton->getRenderer()->setTextColor(tgui::Color(170, 170, 200));
  signInButton->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
  signInButton->getRenderer()->setBackgroundColorHover(sf::Color(45, 45, 60));
  signInButton->getRenderer()->setBorders(1);
  signInButton->getRenderer()->setBorderColor(sf::Color(70, 70, 95));
  signInButton->setPosition("10%", "72%");
  signInButton->setSize("37%", "13%");
  back->add(signInButton);
  signInButton->onPress(&LoginView::signIn, this, editBoxUsername,
                        editBoxPassword);
}

void LoginView::login(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd) {
  auto &manager = app_.getNetwork();
  auto &currentUser_ = app_.getUser();
  currentUser_ = User(static_cast<std::string>(usrname->getText()), 0);
  manager.login(static_cast<std::string>(usrname->getText()),
                static_cast<std::string>(pswd->getText()));
}

void LoginView::signIn(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd) {
  auto &manager = app_.getNetwork();
  auto &currentUser_ = app_.getUser();

  // NOTE: I have to put the condition username 3-15 car

  if (usrname->getText() != "") {
    currentUser_ = User(static_cast<std::string>(usrname->getText()), 0);
    manager.askRegister(static_cast<std::string>(usrname->getText()),
                        static_cast<std::string>(pswd->getText()));
    std::cout << usrname->getText() << "   " << pswd->getText() << std::endl;
  }
}
void LoginView::showError(const std::string &message) {
  auto &gui = app_.getGui();
  auto back = gui.get<tgui::Panel>("Main login");
  if (!back)
    return;
  auto errorLabel = back->get<tgui::Label>("ErrorLabel");
  if (!errorLabel)
    return;
  errorLabel->setText(message);
  errorLabel->getScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
  auto usrname = back->get<tgui::EditBox>("Username");
  auto pswd = back->get<tgui::EditBox>("Password");
  if (usrname)
    usrname->setText("");
  if (pswd)
    pswd->setText("");
}

void LoginView::handleEvents(const sf::Event &event) {

};
