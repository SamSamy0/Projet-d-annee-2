#pragma once
#include <iostream>
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
// unique_ptr<Project> project;
    int id_;
    sf::Vector2u size_;
    unsigned int scale_;
    Zoom zoom_;
    Move move_;
    sf::View viewMap_;
    vector<Layer> layers_;
public:
    Map(int mapId,sf::Vector2u size , unsigned int scale, vector<Layer> layers);
    sf::Vector2u getSize() const;
    unsigned int getScale() const;
    vector<Layer>& getLayers();
    void insertLayer(Layer& layer, int depth);
    void displayMap(sf::RenderWindow& window);
    void closeWindowManager(sf::RenderWindow& window, const sf::Event& ev);
    void detectMovement();
    void detectZooming(sf::Event event);
};
