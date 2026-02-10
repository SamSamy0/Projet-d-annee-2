
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "layer.hpp"

using namespace std;

// #include "move.hpp"

class Map {
    int id_;
    string mapName_;
    float height_ ;
    float width_ ;
    float scale_;
    float positionX_;
    float positionY_;
    //vector<unique_ptr<Layer>> layers_;
public:
    Map(int mapId, string name, float height, float width, float scale);
    float getWidth() const;
    float getHeight() const;
    string getName() const;
    float getScale() const;
    void setPosition(float x, float y) { positionX_ = x; positionY_ = y; }
    //vector<unique_ptr<Layer>>& getLayers();
    //void insertLayer(unique_ptr<Layer> layer, int depth);
    void displayMap(sf::RenderWindow& window);
    //void closeWindowManager(sf::RenderWindow& window, const sf::Event& ev);
    //void detectMovement(const sf::Event& event);
    // void detectZooming();
};
