#pragma once
#include "View.hpp"

class LoginView : public View {
  void init() override;
  void handleEvents(const sf::Event &event) override;

  void login(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  void signIn(tgui::EditBox::Ptr usrname, tgui::EditBox::Ptr pswd);
  void loginWidget();
public:
    LoginView(Application &app_);
};
