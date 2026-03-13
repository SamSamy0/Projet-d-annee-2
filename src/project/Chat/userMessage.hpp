#pragma once
#include <string>
#include <ctime>
#include "chat.hpp"

class UserMessage : public MessageChat {
    string message_;
    static Date currentDate();
    public :
        UserMessage(const string& pseudo, const string& message);
        string getTexte() const;
};