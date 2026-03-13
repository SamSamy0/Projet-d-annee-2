#include "../Window.hpp"
#include "../../project/Chat/userMessage.hpp"

void Window::initChatWidget() {
  float width  = mainWindow.getSize().x;
  float height = mainWindow.getSize().y;

  // Je crée le groupe de widgets pour le Chat
  chatPanel_ = tgui::Panel::create();
  chatPanel_->setSize(width * 0.2, height * 0.35);
  chatPanel_->setPosition(width * 0.01, height * 0.65);
  chatPanel_->getRenderer()->setBackgroundColor(tgui::Color(50, 56, 66));
  chatPanel_->getRenderer()->setBorders({1});
  chatPanel_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  chatPanel_->getRenderer()->setRoundedBorderRadius(10);
  gui.add(chatPanel_);

  // Création du titre chat
  auto chatTitle = tgui::Label::create("Chat");
  chatTitle->setSize(width * 0.2, height * 0.045);
  chatTitle->setPosition(0, 0);
  chatTitle->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
  chatTitle->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
  chatTitle->getRenderer()->setTextColor(tgui::Color::White);
  chatPanel_->add(chatTitle);

  // Je crée la zone de messages, la où ils vont apparaître
  chatMessages_ = tgui::ScrollablePanel::create();
  chatMessages_->setSize(width * 0.18, height * 0.194);
  chatMessages_->setPosition(width * 0.01, height * 0.045);
  chatMessages_->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  chatMessages_->getRenderer()->setBorders({1});
  chatMessages_->getRenderer()->setBorderColor(tgui::Color(70, 75, 85));
  chatMessages_->setHorizontalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
  chatMessages_->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
  chatPanel_->add(chatMessages_);

  // Création de la box pour écrire un message
  chatInput_ = tgui::EditBox::create();
  chatInput_->setSize(width * 0.18, height * 0.05);
  chatInput_->setPosition(width * 0.01, height * 0.247);
  chatInput_->setDefaultText("Entrez message");
  chatInput_->getRenderer()->setBackgroundColor(tgui::Color(36, 40, 47));
  chatInput_->getRenderer()->setBackgroundColorHover(tgui::Color(36, 40, 47));
  chatInput_->getRenderer()->setTextColor(tgui::Color::White);
  chatInput_->getRenderer()->setDefaultTextColor(tgui::Color(130, 135, 145));
  chatInput_->getRenderer()->setBorders({1});
  chatInput_->getRenderer()->setBorderColor(tgui::Color(90, 95, 105));
  chatPanel_->add(chatInput_);

  // Création du bouton envoyer le message
  chatSendButton_ = tgui::Button::create("Envoyer");
  chatSendButton_->setSize(width * 0.18, height * 0.04);
  chatSendButton_->setPosition(width * 0.01, height * 0.302);
  chatSendButton_->setTextSize(12);
  chatSendButton_->getRenderer()->setBackgroundColor(tgui::Color(60, 110, 190));
  chatSendButton_->getRenderer()->setBackgroundColorHover(tgui::Color(75, 130, 210));
  chatSendButton_->getRenderer()->setTextColor(tgui::Color::White);
  chatSendButton_->getRenderer()->setBorders({0});
  chatSendButton_->getRenderer()->setRoundedBorderRadius(8);
  chatSendButton_->onPress([this]() {
    tgui::String text = chatInput_->getText();
    if (text.empty()) return;
    auto msg = std::make_shared<UserMessage>(currentUser_.getUser(), text.toStdString());
    project->getChat().addMessage(msg);
    chatInput_->setText("");
    refreshChat();
  });
  chatPanel_->add(chatSendButton_);
}

void Window::refreshChat() {
  chatMessages_->removeAllWidgets();

  float width  = mainWindow.getSize().x;
  float height = mainWindow.getSize().y;
  auto& messages = project->getChat().getMessages();
  float positionY = 4.0;
  float authorDataHeight = height * 0.022;

  for (const auto& msg : messages) {
    if (msg->getType() == MessageType::USER) {
      Date date = msg->getDate();
      std::string pseudo = msg->getAuthor().getUser();
      std::string authorDate = pseudo + " - " + std::to_string(date.day_) + "/" + std::to_string(date.month_) + "/" + std::to_string(date.year_);

      auto author = tgui::Label::create();
      author->setText(authorDate);
      author->setPosition(width * 0.005, positionY);
      author->getRenderer()->setTextColor(tgui::Color(150, 160, 180));
      author->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
      chatMessages_->add(author);
      author->setTextSize(15);
      positionY += authorDataHeight;

      auto message = tgui::Label::create(msg->getTexte());
      message->setPosition(width * 0.005, positionY);
      message->getRenderer()->setTextColor(tgui::Color::White);
      message->getRenderer()->setBackgroundColor(tgui::Color::Transparent);
      message->setMaximumTextWidth(width * 0.16);
      chatMessages_->add(message);
      positionY += message->getSize().y + 4.0;
    }
  }
  chatMessages_->setVerticalScrollbarValue(chatMessages_->getVerticalScrollbarMaxValue());
}