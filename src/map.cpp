#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "map.hpp"
#include <SFML/Config.hpp>
#include <iostream>

using namespace std;

Map::Map(int mapId, string name, float height, float width, float scale) : id_{mapId}, mapName_{name}, height_{height},  
            width_{width}, scale_{scale}, positionX_{width/2}, positionY_{height/2} {}

float Map::getWidth() const { return width_; }

float Map::getHeight() const { return height_; }

string Map::getName() const { return mapName_; }

float Map::getScale() const { return scale_; }

//vector<Layer>& Map::getLayers() { return layers_; }

//void Map::insertLayer(unique_ptr<Layer> layer, int depth) { layers_.insert(layers_.begin() + depth, layer) ; }

void Map::displayMap(sf::RenderWindow& window) {
    
    // on crée le rectangle qui va représenter la carte vide
    sf::RectangleShape map;
    map.setSize(sf::Vector2f(width_, height_));
    map.setOrigin(sf::Vector2f(width_ / 2.f, height_ / 2.f));
    map.setPosition(sf::Vector2f(positionX_ + width_ / 2.f, positionY_ + height_ / 2.f));
    map.setFillColor(sf::Color(255, 255, 255));

    // on crée la vue de la carte (c'est à dire ce que l'utilisateur voit à l'écran)
    sf::View viewMap;
    viewMap.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
    viewMap.setCenter(sf::Vector2f(positionX_ + (width_ / 2), positionY_ + (height_ / 2)));
    viewMap.zoom(3.0);

    // on règle la fénêtre et on l'affiche
    window.clear(sf::Color(60, 60, 60));
    window.setView(viewMap); 
    window.draw(map);
    window.display();
}