#include "displayWindow.hpp"
#include "../server/clientnetwork.hpp"

int main() {
    // Création du manager réseau (adapter si constructeur différent)
    ClientNetworkManager manager;

    // Création de la fenêtre principale
    Window window(manager);

    // Lancement de la boucle principale
    window.run();

    return 0;
}
