#include "chat.hpp"

using namespace std;

MessageChat::MessageChat(const User& author, const Date& date, MessageType type) : author_(author), date_(date), type_(type) {}

User MessageChat::getAuthor() const { return author_; }
Date MessageChat::getDate() const { return date_; }
MessageType MessageChat::getType() const { return type_; }

void Chat::addMessage(shared_ptr<MessageChat> message) {
    messages_.push_back(message);
}

vector<shared_ptr<MessageChat>> Chat::getMessages() const {
    return messages_;
}
