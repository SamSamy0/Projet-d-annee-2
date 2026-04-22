#pragma once
#include <string>
#include "chat.hpp"

enum class typeNotification {
    CONNEXION,
    DECONNEXION,
    LOCK,
    UNLOCK,
    MASK,
    UNMASK,
};

class SystemNotification : public MessageChat {
    typeNotification typeNotif_;
    public :
        string getTexte() const;
        string getJsonTexte() const;
        SystemNotification(const User& user, const Date& date, const std::string& message);
};