#include "systemNotification.hpp"

string SystemNotification::getTexte() const {
    switch (typeNotif_) {
        case typeNotification::CONNEXION:
            return author_.getUser() + " s'est connecté à la carte à " + getDateTexte() + ".";
        case typeNotification::DECONNEXION:
            return author_.getUser() + " s'est déconnecté de la carte à " + getDateTexte() + ".";
        case typeNotification::LOCK:
            return author_.getUser() + " a verrouillé une couche.";
        case typeNotification::UNLOCK:
            return author_.getUser() + " a déverrouillé une couche.";
    }
    return 0;
}

string SystemNotification::getDateTexte() const {
    Date date = getDate();
    return std::to_string(date.day_) + "/" + std::to_string(date.month_) + "/" +
           std::to_string(date.year_) + " " + std::to_string(date.hour_) + ":" + std::to_string(date.min_);
}

SystemNotification::SystemNotification(const User& user, const Date& date, const typeNotification &notif) 
    : MessageChat(user, date, MessageType::SYSTEM), typeNotif_(notif) {}

SystemNotification::SystemNotification(const User& user, const Date& date, const std::string& message) 
    : MessageChat(user, date, MessageType::SYSTEM) {
    if (message == "CONNEXION") {
        typeNotif_ = typeNotification::CONNEXION;
    } else if (message == "DECONNEXION") {
        typeNotif_ = typeNotification::DECONNEXION;
    } else if (message == "LOCK") {
        typeNotif_ = typeNotification::LOCK;
    } else if (message == "UNLOCK") {
        typeNotif_ = typeNotification::UNLOCK;
    } else if (message == "MASK") {
        typeNotif_ = typeNotification::MASK;
    } else if (message == "UNMASK") {
        typeNotif_ = typeNotification::UNMASK;
    }
}

SystemNotification::SystemNotification(const string &pseudo, const uint userId, const typeNotification &notif) 
    : MessageChat( User(pseudo, userId), currentDate(), MessageType::SYSTEM) {
    if (notif == typeNotification::CONNEXION) {
        typeNotif_ = typeNotification::CONNEXION;
    } else if (notif == typeNotification::DECONNEXION) {
        typeNotif_ = typeNotification::DECONNEXION;
    } else if (notif == typeNotification::LOCK) {
        typeNotif_ = typeNotification::LOCK;
    } else if (notif == typeNotification::UNLOCK) {
        typeNotif_ = typeNotification::UNLOCK;
    } else if (notif == typeNotification::MASK) {
        typeNotif_ = typeNotification::MASK;
    } else if (notif == typeNotification::UNMASK) {
        typeNotif_ = typeNotification::UNMASK;
    }
}

string SystemNotification::getJsonTexte() const {
    switch (typeNotif_) {
        case typeNotification::CONNEXION:
            return "CONNEXION";
        case typeNotification::DECONNEXION:
            return "DECONNEXION";
        case typeNotification::LOCK:
            return "LOCK";
        case typeNotification::UNLOCK:
            return "UNLOCK";
        case typeNotification::MASK:
            return "MASK";
        case typeNotification::UNMASK:
            return "UNMASK";
    }
    return 0;
}