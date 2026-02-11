#include <SFML/Graphics.hpp>
#include "map.hpp"

using namespace std;

int main() {
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Test");
    window.setFramerateLimit(60);

    Map map(4, "hello", 800, 1200, 2); // en supposant que tu as un constructeur correct
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
