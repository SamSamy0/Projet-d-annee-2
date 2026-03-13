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
}