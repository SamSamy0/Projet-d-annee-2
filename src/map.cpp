#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "map.hpp"
#include <SFML/Config.hpp>
#include <iostream>

using namespace std;

Map::Map(int mapId, string name, float height, float width, float scale) : id_{mapId}, mapName_{name}, height_{height},  
            width_{width}, scale_{scale} {
                viewMap_.setSize(sf::Vector2f(width_, height_));
    viewMap_.setCenter(sf::Vector2f(width_ / 2.f, height_ / 2.f));
            }

float Map::getWidth() const { return width_; }

float Map::getHeight() const { return height_; }

string Map::getName() const { return mapName_; }

float Map::getScale() const { return scale_; }

//vector<Layer>& Map::getLayers() { return layers_; }

//void Map::insertLayer(unique_ptr<Layer> layer, int depth) { layers_.insert(layers_.begin() + depth, layer) ; }

void Map::displayMap(sf::RenderWindow& window)
{
    // --- Création de la carte (monde fixe) ---
    sf::RectangleShape map;
    map.setSize(sf::Vector2f(width_, height_));
    map.setOrigin(sf::Vector2f(width_ / 2.f, height_ / 2.f));
    map.setPosition(sf::Vector2f(width_ / 2.f, height_ / 2.f));
    map.setFillColor(sf::Color::White);

    // --- Mise à jour de la vue (caméra) ---
    viewMap_.setCenter(
        sf::Vector2f(
            move_.positionX_ + width_ / 2.f,
            move_.positionY_ + height_ / 2.f
        )
    );

    viewMap_.setSize(
        sf::Vector2f(
            window.getSize().x * zoom_.getZoom(),
            window.getSize().y * zoom_.getZoom()
        )
    );

    // --- Affichage ---
    window.clear(sf::Color(60, 60, 60));
    window.setView(viewMap_);
    window.draw(map);
    window.display();
}



void Map::detectZooming(sf::Event event) {
    if (const auto* mw = event.getIf<sf::Event::MouseWheelScrolled>()) {
        if (mw->delta > 0.f) {
            zoom_.zoomIn();   // molette vers le haut
        }
        else if (mw->delta < 0.f) {
            zoom_.zoomOut();  // molette vers le bas
        }
    }   
}

void Map::detectMovement()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        std::cout << "je suis homo et cette touche a été pressée left" << move_.positionX_ << endl;; 
        move_.goLeft();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        std::cout << "je suis homo et cette touche a été pressée right\n"; 
        move_.goRight();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
        std::cout << "je suis homo et cette touche a été pressée up\n"; 
        move_.goTop();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        std::cout << "je suis homo et cette touche a été pressée down\n"; 
        move_.goDown();

    }
}
