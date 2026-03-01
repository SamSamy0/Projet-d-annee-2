#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "zoom.hpp"
#include "move.hpp"

//TODO: DEFINIR DES VALEURS MAX ET MIN POUR LA SIZE, LA SCALE

using namespace std;

// class Project;
class Layer;

class Map {
    // shared_ptr<Project> project;
    int id_;
    sf::Vector2u size_;
    unsigned int scale_;
    vector<shared_ptr<Layer>> layers_;
    Zoom zoom_;
    Move move_;
    unsigned int selected_;
public:
    Map(int mapId,sf::Vector2u size , unsigned int scale, vector<shared_ptr<Layer>> layers);
    Map(int mapId,sf::Vector2u size , unsigned int scale);
    sf::Vector2u getSize() const;
    unsigned int getScale() const;
    bool hasLayer() const;
    vector<shared_ptr<Layer>>& getLayers();
    Zoom& getZoom();
    shared_ptr<Layer> getCurrentLayer();
    void insertLayer(shared_ptr<Layer> layer);
    void createPixelLayer();
    void displayMap(sf::RenderWindow& window, sf::View& viewMap);
    void closeWindowManager(sf::RenderWindow& window, const sf::Event& ev);
    void detectMovement();
    void zooming(sf::Event::MouseWheelScrolled const* event);
    bool isInside(sf::Vector2i pos)const;
    void setLayerSelected(unsigned int i);
    void detectZooming(sf::Event event);
    unsigned int getLayerSelected() const;
};


