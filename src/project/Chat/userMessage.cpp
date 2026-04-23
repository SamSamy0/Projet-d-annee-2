#include "userMessage.hpp"

UserMessage::UserMessage(const string &pseudo, const uint id,
                         const string &message)
    : MessageChat(User(pseudo, id), currentDate(), MessageType::USER),
      message_(message) {}

string UserMessage::getTexte() const { return message_; }

string UserMessage::getJsonTexte() const {return message_;}

UserMessage::UserMessage(const User &author, const Date &date,
                         const string &message)
    : MessageChat(author, date, MessageType::USER), message_(message) {}
