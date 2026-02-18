#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>

#include "project.hpp"

enum class projectState
{
    MENU,
    DRAW,
    ERASE
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({1200, 800}), "Project Test");
    window.setFramerateLimit(60);

    tgui::Gui gui(window);

    Project project(5, {500, 500}, window, gui);

    project.getToolBar().selectTool(PIXELBRUSH);
    project.getToolBar().getSelectedTool()->setColor(sf::Color(0,0,255,125));
    project.getToolBar().getSelectedTool()->setShape(CIRCLE);
    project.getToolBar().getSelectedTool()->setSize(1,1);
    project.getToolBar().getSelectedTool()->drawOn(sf::Vector2i(200,200));

    projectState state = projectState::MENU;

    // 🔹 Boutons (comme avant)
    auto homeButton = tgui::Button::create();
    homeButton->setSize(30, 30);
    homeButton->setPosition(15, 5);
    homeButton->getRenderer()->setTexture("../../res/images/accueil.png");
    homeButton->getRenderer()->setBorders({0});
    homeButton->onPress([&](){
        state = projectState::MENU;
        std::cout << "Home pressed\n";
    });
    gui.add(homeButton);

    auto pencilButton = tgui::Button::create();
    pencilButton->setSize(25, 25);
    pencilButton->setPosition(15, 80);
    pencilButton->getRenderer()->setTexture("../../res/images/crayon.png");
    pencilButton->getRenderer()->setBorders({0});
    pencilButton->onPress([&](){
        project.getToolBar().selectTool(PIXELBRUSH);
    });
    gui.add(pencilButton);

    auto eraseButton = tgui::Button::create();
    eraseButton->setSize(25, 25);
    eraseButton->setPosition(15, 150);
    eraseButton->getRenderer()->setTexture("../../res/images/la-gomme.png");
    eraseButton->getRenderer()->setBorders({0});
    eraseButton->onPress([&](){
        state = projectState::ERASE;
        std::cout << "Erase pressed\n";
    });
    gui.add(eraseButton);

    // 🔹 Boucle principale
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            // 👉 Détection zoom
            project.getMap()->detectZooming(*event);

            gui.handleEvent(*event);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
                sf::Vector2i mousePos = sf::Mouse::getPosition(window); //position de la souris
                sf::Vector2f pos = window.mapPixelToCoords(mousePos, project.getView());
                sf::Vector2i mapPos(static_cast<int>(pos.x), static_cast<int>(pos.y));

                if (project.getMap()->isInside(mapPos) && !gui.getWidgetAtPos(sf::Vector2f(mousePos),true)) {
                    project.getToolBar().getSelectedTool()->drawOn(mapPos);
                }
            }
        }

        // 👉 Détection mouvement (hors boucle event)
        project.getMap()->detectMovement();

        // 👉 Affichage
        project.display();
    }

    return 0;
}
