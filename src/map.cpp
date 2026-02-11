#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "map.hpp"
#include <SFML/Config.hpp>
#include <iostream>

using namespace std;

Map::Map(int mapId, string name, float height, float width, float scale) : id_{mapId}, mapName_{name}, height_{height}, width_{width}, scale_{scale}, move_{width, height} {
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
    
    // On crée la map => zone dessinable
    sf::RectangleShape map;
    map.setSize(sf::Vector2f(width_, height_));
    map.setOrigin(sf::Vector2f(width_ / 2, height_ / 2));
    map.setPosition(sf::Vector2f(width_ / 2, height_ / 2));
    map.setFillColor(sf::Color::White);
    
    // Pour éviter que SFML n'étire la carte si celle-ci n'a pas la meme taille que la fenêtre, on utilise un système de ratio
    // Note : idée proposée par ChatGPT !!
    float windowRatio = static_cast<float>(window.getSize().x) / window.getSize().y;
    float mapRatio = width_ / height_;
    float zoomFactor = zoom_.getZoom();

    if (windowRatio > mapRatio) {
        float newWidth = height_ * windowRatio;
        viewMap_.setSize(sf::Vector2f(newWidth * zoomFactor, height_ * zoomFactor));
    } else {
        float newHeight = width_ / windowRatio;
        viewMap_.setSize(sf::Vector2f(width_ * zoomFactor, newHeight * zoomFactor));
    }
    // Fin de l'idée de ChatGPT
    
    viewMap_.setCenter(sf::Vector2f(move_.positionX_ + width_ / 2, move_.positionY_ + height_ / 2));

    // On règle la fenêtre et on l'affiche
    window.clear(sf::Color(60, 60, 60));
    window.setView(viewMap_);
    window.draw(map);
    window.display();
}

void Map::detectZooming(sf::Event event) {
    if (const auto* mouse = event.getIf<sf::Event::MouseWheelScrolled>()) {
        if (mouse->delta > 0) { // regarde si l'utilisateur veut faire un zoom avant
            zoom_.zoomIn(); 
        }
        else if (mouse->delta < 0) {  // regarde si l'utilisateur veut faire un zoom arrière
            zoom_.zoomOut();
        }
    }  
}

void Map::detectMovement()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))) { // regarde si l'utilisateur veut aller à gauche
        move_.goLeft(zoom_.getZoom());
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))) { // regarde si l'utilisateur veut aller à droite
        move_.goRight(zoom_.getZoom());
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))) { // regarde si l'utilisateur veut aller en haut
        move_.goTop(zoom_.getZoom());
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))) { // regarde si l'utilisateur veut aller en bas
        move_.goDown(zoom_.getZoom());
    }
}
