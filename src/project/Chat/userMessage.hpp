#pragma once
#include <string>
#include <ctime>
#include "chat.hpp"

class UserMessage : public MessageChat {
    string message_;
    static Date currentDate() {
        time_t t = time(nullptr);
        tm* now = localtime(&t);
        return Date(now->tm_mday, now->tm_mon + 1, now->tm_year - 100);
    }
    public :
        UserMessage(const string& pseudo, const string& message)
            : MessageChat(User(pseudo), currentDate(), MessageType::USER), message_(message) {}
        string getTexte() const;
};