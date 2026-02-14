#include <SFML/Graphics.hpp>
#include "map.hpp"
#include "layer.hpp"

using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Test", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    vector<shared_ptr<Layer>> layers;

    Map map(4,sf::Vector2u(1200,800), 2,layers); // en supposant que tu as un constructeur correct
    // map.setWidthHeight... etc si besoin

    while (window.isOpen()) {
        while (auto ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) {
                window.close();
            }
            map.detectZooming(*ev);
        }
        map.detectMovement();
        map.displayMap(window); // dessiner à chaque frame
    }

    return 0;
}
