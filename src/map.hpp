#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "layer.hpp"

using namespace std;

class Moving;

class Map {
    Moving move_;
    int id;
    string mapName_;
    float height_;
    float width_;
    float scale_;
    vector<Layer> layers_;
    public :
        float getWidth() const ;
        float getHeight() const ;
        string getName() const ;
        float getScale() const ;
        vector<Layer>& getLayers() ;
        void insertLayer(const Layer& layer, int depth) ;
        void displayMap(sf::RenderWindow& window) ;
        void detectMovement() ;
        void detectZooming() ;
        void loadFromDiks();
        void saveToDisk();
};
