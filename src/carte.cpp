#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

enum class typeOutilsPixel {
    PINCEAU,
    GOMME,
    DEPLACER_COUCHE 
};

// ----- Liste des classes pour la visibilité -----

class EchelleMetrePixel;
class Carte;
class Vue;
class VuePrincipale;
class Deplacement;
class GestionSynchronisation;
class GestionOutils;
class Zoom;

class EchelleMetrePixel {
    float metreParPixel_;
};

class Vue {
    float positionX_;
    float positionY_;
    Zoom zoom_;
    public :
        void setPositionX(float x) { positionX_ = x; }
        void setPositionY(float y) { positionY_ = y; }
        void setZoom(float zoom) { zoom_.setFacteurZoom(zoom); }
        float getPositionX() { return positionX_; }
        float getPositionY() { return positionY_; }
        float getPositionX() { return zoom_.getZoom(); }
};

class Carte {
    int id;
    string nomCarte;
    float hauteurCarte;
    float largeurCarte_;
    EchelleMetrePixel echelle_;
    vector<Couche> couches_;
    public :
        float getHauteurCarte() { return hauteurCarte_; }
        float getLargeurCarte() { return largeurCarte_; }
        string getNomCarte() { return nomCarte_;}
        EchelleMetrePixel& getEchelle() { echelle_.getEchelle(); }
        vector<Couche>& getCouches() { return couches_; }
        void ajouterCouche(Couche couche) { couches.push_back(couche); }
};

class VuePrincipale : public Vue {
    Deplacement seDeplacer;
    Carte carte_;
    GestionSynchronisation sync_;
    GestionOutils outils_;
    public :
        void gestionCarte() {
            sf::RenderWindow fenetre(sf::VideoMode(1280, 720), carte_.getNomCarte()); // création de la fenêtre du projet
            while (fenetre.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    fenetre.closed();
                }
                afficherCarte(fenetre);
                captureInteractions();
            }
        }
        void afficherCarte(sf::RenderWindow& fenetre) {
            fenetre.clear();
            sf::View vueCarte;
            vueCarte.setSize(carte_.getLargeurCarte(), carte_.getHauteurCarte());
            vueCarte.setCenter(positionX_, positionY_);
            vueCarte.zoom(zoom_.getZoom());
            fenetre.display();
        }
        void captureInteractions() {
            if (event.type == sf::Event::KeyPressed) { // regarde si un event c'est produit (l'utilisateur a appuyé sur une touche)
                switch(event.key.code) {
                    case sf::Keyboard::Z || sf::Keyboard::Up : // check s'il veut monter
                        seDeplacer.allerHaut();
                        break;
                    case sf::Keyboard::Q || sf::Keyboard::Left : // check s'il veut aller à gauche
                        seDeplacer.allerGauche();
                        break;
                    case sf::Keyboard::S || sf::Keyboard::Bas : // check s'il veut aller à droite
                        seDeplacer.allerBas();
                        break;
                    default : // check s'il veut descendre
                        seDeplacer.allerDroite();
                }
        
            }
        }
};

class Deplacement {
    View& vue_;
    float vitesseDeplacement;
    public :
        void allerGauche() {}
        void allerDroite() {}
        void allerHaut() {}
        void allerBas() {}
};

class Zoom {
    float facteurZoom_;
    public :
        void setFacteurZoom(float zoom) { facteurZoom_ = zoom; }
        float getZoom() { return facteurZoom_; }
        void zoomIn() {}
        void zoomOut() {}
};