#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <optional>

using std::string;
using std::vector;

enum class typeOutilsPixel {
    PINCEAU,
    GOMME,
    DEPLACER_COUCHE 
};

const float maxZoom = 4.0;
const float minZoom = 0.25;

// ----- Liste des classes pour la visibilité -----

class EchelleMetrePixel;
class Carte;
class Vue;
class VuePrincipale;
class Deplacement;
class GestionSynchronisation;
class GestionOutils;
class Zoom;
class Couche;

class Couche {};

class EchelleMetrePixel {
    float metreParPixel_;
};



class Vue {
    protected:
        float positionX_{0};
        float positionY_{0};
        Zoom zoom_;
    public :
        void setPositionX(float x) { positionX_ = x; }
        void setPositionY(float y) { positionY_ = y; }
        void setZoom(float zoom) { zoom_.setFacteurZoom(zoom); }
        float getPositionX() { return positionX_; }
        float getPositionY() { return positionY_; }
        float getZoom() { return zoom_.getZoom(); }
};

class Deplacement {
    Vue& vue_;
    float vitesseDeplacement{1.f};
    public :
        Deplacement(Vue& vue) : vue_(vue) {}
        void allerGauche() {}
        void allerDroite() {}
        void allerHaut() {}
        void allerBas() {}
};

class GestionSynchronisation {};
class GestionOutils {};

class Carte {
    int id;
    string nomCarte_;
    float hauteurCarte_;
    float largeurCarte_;
    EchelleMetrePixel echelle_;
    vector<Couche> couches_;
    public :
        float getHauteurCarte() { return hauteurCarte_; }
        float getLargeurCarte() { return largeurCarte_; }
        string getNomCarte() { return nomCarte_;}
        EchelleMetrePixel& getEchelle() { return echelle_; }
        vector<Couche>& getCouches() { return couches_; }
        void ajouterCouche(const Couche& couche) { couches_.push_back(couche); }
};

class VuePrincipale : public Vue {
    Deplacement seDeplacer;
    Carte carte_;
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

int main() {
    sf::RenderWindow fenetre(sf::VideoMode(sf::Vector2u(800u, 600u)), "Test SFML - Fenetre");

    // Optionnel : limite FPS pour éviter de cramer le CPU
    fenetre.setFramerateLimit(60);

    while (fenetre.isOpen()) {
        while (const std::optional event = fenetre.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                fenetre.close();
            }
        }

        fenetre.clear(sf::Color::Black);
        fenetre.display();
    }

    std::cout << "Fenetre fermee, programme termine.\n";
    return 0;
}
