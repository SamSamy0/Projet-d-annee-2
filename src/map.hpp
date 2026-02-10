
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

// #include "layer.hpp"
// #include "move.hpp"

class Map {
    sf::Color backgroundColor_ = sf::Color(245, 245, 245);
    int id{};
    std::string mapName_;
    float height_ = 800.f;
    float width_ = 1900.f;
    float scale_{1.f};
    float positionX_ = 0.f;
    float positionY_ = 0.f;
    // std::vector<Layer> layers_;
public:
    float getWidth() const;
    float getHeight() const;
    std::string getName() const;
    float getScale() const;

    void setPosition(float x, float y) { positionX_ = x; positionY_ = y; }

    // std::vector<Layer>& getLayers();
    // void addLayer(const Layer& layer);
    void displayMap(sf::RenderWindow& window);
    void detectMovement(const sf::Event& event);
    // void detectZooming();
};
