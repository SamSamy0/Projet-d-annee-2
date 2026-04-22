#pragma once
#include "chat.hpp"
#include <ctime>
#include <string>

class UserMessage : public MessageChat {
  string message_;
  static Date currentDate();

public:
  UserMessage(const string &pseudo, const uint userId, const string &message);
  string getTexte() const;
  string getJsonTexte() const;
  UserMessage(const User &author, const Date &date, const string &message);
};
