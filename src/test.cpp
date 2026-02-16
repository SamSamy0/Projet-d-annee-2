#include <SFML/Graphics.hpp>
#include <memory>
#include "project.hpp"
#include "map.hpp"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({1200, 800}),
        "Project Test",
        sf::Style::Titlebar | sf::Style::Close
    );

    window.setFramerateLimit(60);

    // 🔹 Création du projet
    Project project(1, sf::Vector2u(500, 500), window);

    std::shared_ptr<Map> map = project.getMap();

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // Gestion du zoom
            map->detectZooming(*event);
        }

        // Gestion du déplacement
        map->detectMovement();

        // Affichage complet via Project
        project.display();
    }

    return 0;
}
