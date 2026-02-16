#include "app.hpp"
#include <SFML/Graphics.hpp>
#include "state.hpp"

App::App(): appState_(State::AUTH){}

void App::run() {
    //--------[ creation des objets ]--------//

    /* PENSER A CREER ICI TOUT LES OBJET NECESSAIRE DES LE DEBUT */
    sf::RenderWindow appWindow;
    ClientNetworkManager to_server;
    Handler handler = Handler(to_server);


    //--------[ se connecter au serveur ]--------//

    to_server.connect();


    //--------[ boucle principale ]--------//

    while (appWindow.isOpen()) {
        // [ recevoir les messages du serveur ] //
        to_server.getAnswers();
        
        // [ traiter les messages du serveur ] //
        handler.process(); 
        
        // [ boucle principale ] //
        switch(appState_) {

            // !!!!! Attention update...() et render() sont des noms aleatoire pour 
            // expliqué ce qui est fait biensur mettez vos propre objet avec methode, votre code, ...
            // pensez orienté objet

            case State::AUTH:
            /*
            Ici c'est pour Samy il me semble tu doit gerer tout ce qui est 
            en rapport a la fenetre d authentification
            */
                updateAuth(); //capter les entrée utilisateur reagir en consequence -> event 
                render(); // afficher le rendu de la fenetre
                break;

            case State::LOBBY:
            /* 
            ici aussi c'est pour toi Samy, je crois, meme chose que précédament mais pour le lobby, 
            Courage! 
            */

                updateLobby(); //capter les entrée utilisateur reagir en consequence -> event 
                render(); // afficher le rendu de la fenetre
                break;

            case State::PROJET:
            /*
            Adam et Naim ca c'est pour vous, vous l'aurez compris tout ce qui concerne 
            un projet (lorsqu'on est dedans) ce passe ici
            */
                updateProjet(); // afficher le rendu de la fenetre
                render();   // afficher le rendu de la fenetre
                break;
        }

        
    }
}
