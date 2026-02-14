#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "layer.hpp"
#include "zoom.hpp"
#include "move.hpp"

//TODO: DEFINIR DES VALEURS MAX ET MIN POUR LA SIZE, LA SCALE

using namespace std;

class Project;

class Map {
    // shared_ptr<Project> project;
    int id_;
    sf::Vector2u size_;
    unsigned int scale_;
    Zoom zoom_;
    Move move_;
    sf::View viewMap_;
    vector<shared_ptr<Layer>> layers_;
public:
    Map(int mapId,sf::Vector2u size , unsigned int scale, vector<shared_ptr<Layer>> layers);
    sf::Vector2u getSize() const;
    unsigned int getScale() const;
    vector<shared_ptr<Layer>>& getLayers();
    void insertLayer(shared_ptr<Layer> layer, int depth);
    void displayMap(sf::RenderWindow& window);
    void closeWindowManager(sf::RenderWindow& window, const sf::Event& ev);
    void detectMovement();
    void detectZooming(sf::Event event);
};
