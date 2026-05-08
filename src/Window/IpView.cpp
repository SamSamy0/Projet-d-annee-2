#include "IpView.hpp"
#include "Application.hpp"
#include "LoginView.hpp"

void IpView::init() {
  auto &gui = app_.getGui();

  auto logo = tgui::Picture::create("../res/images/logo.png");
  logo->setSize("24%", "8%");
  logo->setPosition("38%", "22%");
  gui.add(logo);

  auto background = tgui::Panel::create();
  background->setSize("32%", "30%");
  background->setPosition("34%", "38%");
  background->getRenderer()->setBackgroundColor(tgui::Color(28, 28, 36));
  background->getRenderer()->setRoundedBorderRadius(12);
  background->getRenderer()->setBorderColor(sf::Color(55, 55, 70));
  gui.add(background, "IpPanel");

  auto title = tgui::Label::create("Entrez l'IP du serveur");
  title->setSize("100%", "15%");
  title->setPosition("0%", "8%");
  title->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
  title->setTextSize(22);
  title->getRenderer()->setTextColor(sf::Color(240, 240, 255));
  background->add(title);

  error_ = tgui::Label::create("");
  error_->setPosition({"10%", "22%"});
  error_->setSize({"80%", "13%"});
  error_->getRenderer()->setTextColor(sf::Color(220, 80, 80));
  error_->setTextSize(9);
  background->add(error_);

  auto ip = tgui::EditBox::create();
  ip->setPosition({"10%", "38%"});
  ip->setSize({"80%", "13%"});
  ip->setDefaultText("127.0.0.1");
  ip->getRenderer()->setBackgroundColor(sf::Color(20, 20, 28));
  ip->getRenderer()->setBackgroundColorHover(sf::Color(24, 24, 34));
  ip->getRenderer()->setTextColor(sf::Color::White);
  ip->getRenderer()->setDefaultTextColor(sf::Color(90, 90, 115));
  ip->getRenderer()->setBorders(1);
  ip->getRenderer()->setBorderColor(sf::Color(55, 55, 75));
  ip->getRenderer()->setRoundedBorderRadius(6);
  background->add(ip, "IpBox");

  auto connect = tgui::Button::create("Se connecter");
  connect->getRenderer()->setBackgroundColor(sf::Color(99, 102, 241));
  connect->getRenderer()->setBackgroundColorHover(sf::Color(118, 120, 255));
  connect->getRenderer()->setTextColor(sf::Color::White);
  connect->getRenderer()->setBorders(0);
  connect->getRenderer()->setRoundedBorderRadius(8);
  connect->setPosition({"25%", "64%"});
  connect->setSize({"50%", "18%"});
  background->add(connect);
  connect->onPress(&IpView::tryConnect, this, ip);
}

void IpView::tryConnect(tgui::EditBox::Ptr ipBox) {
  std::string ip = static_cast<std::string>(ipBox->getText());
  if (ip.empty()) ip = "127.0.0.1"; // ip par défaut si on lance en local

  if (app_.getNetwork().connect(ip, 5001)) {
    app_.changeView(std::make_unique<LoginView>(app_));
  } else {
    error_->setText("Connexion impossible");
  }
}
