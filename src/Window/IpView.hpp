#pragma once
#include "View.hpp"
#include <TGUI/Widgets/EditBox.hpp>

class IpView : public View {
public:
  IpView(Application &app) : View(app) {};
  void init() override;
  void handleEvents(const sf::Event &event) override {};

private:
  tgui::Label::Ptr error_;
  void tryConnect(tgui::EditBox::Ptr ipBox);
};
