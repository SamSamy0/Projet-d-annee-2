#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <memory>
#include "project.hpp"
#include "tool.hpp"
#include "map.hpp"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode::getDesktopMode(),
        "Project Test",
        sf::State::Fullscreen
    );

    tgui::Gui gui(window);

    window.setFramerateLimit(60);

    // 🔹 Création du projet
    Project project(10, sf::Vector2u(5000, 5000), window, gui);

    std::shared_ptr<Map> map = project.getMap();

    project.getToolBar().selectTool(PIXELBRUSH);
    project.getToolBar().getSelectedTool()->setColor(sf::Color(0,0,255,125));
    project.getToolBar().getSelectedTool()->setShape(CIRCLE);
    project.getToolBar().getSelectedTool()->setSize(1,1);
    project.getToolBar().getSelectedTool()->drawOn(sf::Vector2i(-25,200));

    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // Gestion du zoom
            map->detectZooming(*event);
            gui.handleEvent(*event);




      if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
        sf::Vector2i mousePos = sf::Mouse::getPosition(window); //position de la souris
        sf::Vector2f pos = window.mapPixelToCoords(mousePos, project.getView());
        sf::Vector2i mapPos(static_cast<int>(pos.x), static_cast<int>(pos.y));


      if (project.getMap()->isInside(mapPos) && !gui.getWidgetAtPos(sf::Vector2f(mousePos),true)) {
                  project.getToolBar().getSelectedTool()->drawOn(mapPos);
        }
        }

        // Gestion du déplacement
        map->detectMovement();

        // Affichage complet via Project
        project.display();
    }
        }

    return 0;
}
