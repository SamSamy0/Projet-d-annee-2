#pragma once
#include <vector>
#include <string>
#include <memory>
#include "../user.hpp"
#include "date.hpp"

using namespace std;

enum class MessageType {
    USER,
    SYSTEM,
};

class MessageChat {
    protected :
        User author_;
        Date date_;
        MessageType type_;
    public :
        MessageChat() = default;
        MessageChat(const User& author, const Date& date, MessageType type)
            : author_(author), date_(date), type_(type) {}
        virtual string getTexte() const = 0;
        User getAuthor() const { return author_; }
        Date getDate() const { return date_; }
        MessageType getType() const { return type_; }
};

class Chat {
    vector<shared_ptr<MessageChat>> messages_;
    public :
        void addMessage(shared_ptr<MessageChat> message);
        vector<shared_ptr<MessageChat>> getMessages() const;
};