#include <SFML/Graphics.hpp>
#include <memory>
#include "tool.hpp"
#include "map.hpp"
#include "layer.hpp"
#include "project.hpp"

using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Test", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    vector<shared_ptr<Layer>> layers;
    

    Project project(5,sf::Vector2u(500,500));
    shared_ptr<Map> map = project.getMap();

    std::shared_ptr<Tool> pixelbrush = project.getTools()[PIXELBRUSH];
    auto pb = std::dynamic_pointer_cast<PixelBrush>(pixelbrush);
    pb->setColor(sf::Color(255,0,0,255));
    pb->setShape(SQUARE);
    pb->setSize(10,0);
    pb->drawOn(sf::Vector2u(200,200));

    while (window.isOpen()) {
        while (auto ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) {
                window.close();
            }
            map->detectZooming(*ev);
        }
        map->detectMovement();
        window.clear();
        map->displayMap(window); // dessiner à chaque frame
        window.display();
    }

    return 0;
}
