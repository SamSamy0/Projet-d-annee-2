#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "map.hpp"

using namespace std;

// ----- Implémentation des méthodes Map -----

float Map::getWidth() const { return width_; }
float Map::getHeight() const { return height_; }
string Map::getName() const { return mapName_; }
float Map::getScale() const { return scale_; }
//vector<Layer>& Map::getLayers() { return layers_; }
//void Map::insertLayer(const Layer& layer, int depth) { layers_.insert(layers_.begin() + depth, layer) ; }
void Map::displayMap(sf::RenderWindow& window) {
     // 1) Création de la view (caméra)
    sf::View view;
    view.setSize(width_, height_);
    view.setCenter(positionX_, positionY_);

    // 2) Création de la carte (zone de base)
    sf::RectangleShape map;
    map.setSize(sf::Vector2f(width_, height_));
    map.setPosition(positionX_, positionY_);
    map.setFillColor(sf::Color(245, 245, 245));

    // 3) Clear de la fenêtre (fond gris)
    window.clear(sf::Color(60, 60, 60));

    // 4) Application de la view
    window.setView(view);

    // 5) Dessin de la carte
    window.draw(map);
}
//void Map::detectMovement() {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) { // regarde si un event c'est produit (l'utilisateur a appuyé sur une touche)
                switch (keyPressed->code) {
                    case sf::Keyboard::Key::Z || sf::Keyboard::Key::Up // check s'il veut monter
                        move_.goTop();
                        break;
                    case sf::Keyboard::Key::Q || sf::Keyboard::Key::Left // check s'il veut aller à gauche
                        move_.goLeft();
                        break;
                    case sf::Keyboard::Key::D || sf::Keyboard::Key::Right // check s'il veut aller à droite
                        move_.goRight();
                        break;
                    default: // check s'il veut descendre
                        move_.goDown();
                }
        
            }
        //}


int main() {

    // Création de la fenêtre
    sf::RenderWindow window(
        sf::VideoMode(1200, 800),
        "Test affichage Map"
    );
    window.setFramerateLimit(60);

    // Création de la carte
    // largeur = 1900, hauteur = 800
    Map map;

    // Position de la carte (au choix)
    map.setPosition(100.f, 100.f);

    // Boucle principale
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Affichage de la carte
        map.displayMap(window);

        // Affichage final
        window.display();
    }

    return 0;
}
