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
unsigned int Map::getScale() const { return scale_; }
vector<Layer>& Map::getLayers() { return layers_; }
void Map::insertLayer(const Layer& layer, int depth) { layers_.insert(layers_.begin() + depth, layer) ; }
void Map::displayMap(sf::RenderWindow& window) {
    window.clear();
    sf::View mapView;
    mapView.setSize(sf::Vector2f(width_, height_));
    mapView.setCenter(sf::Vector2f(positionX_, positionY_));
    mapView.zoom(zoom_.getZoom());
    window.setView(mapView);
    window.display();
}
void Map::detectMovement() {
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
}
