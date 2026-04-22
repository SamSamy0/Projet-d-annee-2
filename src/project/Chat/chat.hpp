#pragma once
#include "../user.hpp"
#include "date.hpp"
#include <memory>
#include <string>
#include <vector>
#include <QJsonArray>

using namespace std;

enum class MessageType {
  USER,
  SYSTEM,
};

class MessageChat {
protected:
  User author_;
  Date date_;
  MessageType type_;

public:
  MessageChat() = default;
  MessageChat(const User &author, const Date &date, MessageType type);
  virtual string getTexte() const = 0;
  virtual string getJsonTexte() const = 0;
  User getAuthor() const;
  Date getDate() const;
  MessageType getType() const;
};

class Chat {
  vector<shared_ptr<MessageChat>> messages_;

public:
  void addMessage(shared_ptr<MessageChat> message);
  vector<shared_ptr<MessageChat>> getMessages() const;
  Chat(const QJsonArray& jsonArray);
  Chat() = default;
  QJsonArray toJson() const;
};
