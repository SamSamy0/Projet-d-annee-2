#include <SFML/Graphics.hpp>
#include <memory>
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

    while (window.isOpen()) {
        while (auto ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) {
                window.close();
            }
            map->detectZooming(*ev);
        }
        map->detectMovement();
        map->displayMap(window); // dessiner à chaque frame
    }

    return 0;
}
