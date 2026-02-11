
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "layer.hpp"
#include "zoom.hpp"
#include "move.hpp"

using namespace std;

// #include "move.hpp"

class Map {
    int id_;
    string mapName_;
    float height_ ;
    float width_ ;
    float scale_;
    Zoom zoom_;
    Move move_;
    sf::View viewMap_;
    //vector<unique_ptr<Layer>> layers_;
public:
    Map(int mapId, string name, float height, float width, float scale);
    float getWidth() const;
    float getHeight() const;
    string getName() const;
    float getScale() const;
    //vector<unique_ptr<Layer>>& getLayers();
    //void insertLayer(unique_ptr<Layer> layer, int depth);
    void displayMap(sf::RenderWindow& window);
    //void closeWindowManager(sf::RenderWindow& window, const sf::Event& ev);
    void detectMovement();
    void detectZooming(sf::Event event);
};
