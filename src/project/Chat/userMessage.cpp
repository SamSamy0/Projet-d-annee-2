#include "userMessage.hpp"

Date UserMessage::currentDate() {
  time_t t = time(nullptr);
  tm *now = localtime(&t);
  return Date(now->tm_mday, now->tm_mon + 1, now->tm_year - 100);
}

UserMessage::UserMessage(const string &pseudo, const uint id,
                         const string &message)
    : MessageChat(User(pseudo, id), currentDate(), MessageType::USER),
      message_(message) {}

string UserMessage::getTexte() const { return message_; }

string UserMessage::getJsonTexte() const {return message_;}

UserMessage::UserMessage(const User &author, const Date &date,
                         const string &message)
    : MessageChat(author, date, MessageType::USER), message_(message) {}
