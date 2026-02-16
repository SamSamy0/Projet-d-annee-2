#include <SFML/Graphics.hpp>
#include <memory>
#include "project.hpp"
#include "map.hpp"
#include "layer.hpp"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({1200, 800}),
        "Map Test",
        sf::Style::Titlebar | sf::Style::Close
    );

    window.setFramerateLimit(60);

    // 🔹 Création du projet
    Project project(1, sf::Vector2u(500, 500));

    // 🔹 Récupération de la map
    std::shared_ptr<Map> map = project.getMap();

    // 🔹 S'assurer qu'il y a au moins une layer
    if (!map->hasLayer())
        map->createPixelLayer();

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            map->detectZooming(*event);
        }

        map->detectMovement();

        // ⚠️ UN SEUL clear
        window.clear(sf::Color(60, 60, 60));

        // ⚠️ displayMap ne doit PAS faire clear() ni display()
        map->displayMap(window);

        // ⚠️ UN SEUL display
        window.display();
    }

    return 0;
}
