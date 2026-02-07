#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;

class Vue {
    protected:
        float x_{0};
        float y_{0};
        Zoom zoom_;
    public :
        void setPositionX(float x) { positionX_ = x; }
        void setPositionY(float y) { positionY_ = y; }
        void setZoom(float zoom) { zoom_.setFacteurZoom(zoom); }
        float getPositionX() { return positionX_; }
        float getPositionY() { return positionY_; }
        float getZoom() { return zoom_.getZoom(); }
};

class VuePrincipale : public Vue {
    Moving move_;
    Map map_;
    GestionSynchronisation sync_;
    GestionOutils outils_;
    public :
        VuePrincipale() : seDeplacer(*this) {}
        void gestionCarte() {
            sf::RenderWindow fenetre(sf::VideoMode(sf::Vector2u(1280u, 720u)), carte_.getNomCarte()); // création de la fenêtre du projet
            while (fenetre.isOpen()) {
                while (const std::optional event = fenetre.pollEvent()) { // regarde si l'utilisateur n'a pas fermé la page
                    if (event->is<sf::Event::Closed>()) {
                        fenetre.close();
                    }
                    captureInteractions(*event);
                }
                afficherCarte(fenetre);
            }
        }
        void afficherCarte(sf::RenderWindow& fenetre) {
            fenetre.clear();
            sf::View vueCarte;
            vueCarte.setSize(sf::Vector2f(carte_.getLargeurCarte(), carte_.getHauteurCarte()));
            vueCarte.setCenter(sf::Vector2f(positionX_, positionY_));
            vueCarte.zoom(zoom_.getZoom());
            fenetre.setView(vueCarte);
            fenetre.display();
        }
        void captureInteractions(const sf::Event& event) {
            if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) { // regarde si un event c'est produit (l'utilisateur a appuyé sur une touche)
                switch (keyPressed->code) {
                    case sf::Keyboard::Key::Z:
                    case sf::Keyboard::Key::Up: // check s'il veut monter
                        seDeplacer.allerHaut();
                        break;
                    case sf::Keyboard::Key::Q:
                    case sf::Keyboard::Key::Left: // check s'il veut aller à gauche
                        seDeplacer.allerGauche();
                        break;
                    case sf::Keyboard::Key::S:
                    case sf::Keyboard::Key::Down: // check s'il veut aller à droite
                        seDeplacer.allerBas();
                        break;
                    default: // check s'il veut descendre
                        seDeplacer.allerDroite();
                }
        
            }
        }
};