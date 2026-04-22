#include "systemNotification.hpp"

string SystemNotification::getTexte() const {
    switch (typeNotif_) {
        case typeNotification::CONNEXION:
            return author_.getUser() + "s'est connecté à la carte.";
        case typeNotification::DECONNEXION:
            return author_.getUser() + "s'est déconnecté de la carte.";
        case typeNotification::LOCK:
            return author_.getUser() + "a verrouillé une couche.";
        case typeNotification::UNLOCK:
            return author_.getUser() + "a déverrouillé une couche.";
    }
    return 0;
}

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