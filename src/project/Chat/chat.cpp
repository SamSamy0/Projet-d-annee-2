#include "chat.hpp"

using namespace std;

void Chat::addMessage(shared_ptr<MessageChat> message) {
    messages_.push_back(message);
}

vector<shared_ptr<MessageChat>> Chat::getMessages() const {
    return messages_;
}
