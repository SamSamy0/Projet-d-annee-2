#include "../Application.hpp"
#include "GameView.hpp"

void GameView::initMinimap() {
}

void GameView::drawMinimap() {
    auto& mainWindow = app_.getWindow();
    auto map = project->getMap();
    auto& texture = map->getRenderTexture().getTexture();
    sf::Vector2u mapSize = map->getSize();

    float scale = ((200.f / mapSize.x) < (150.f / mapSize.y)) ? (200.f / mapSize.x) : (150.f / mapSize.y);
    float width = mapSize.x * scale;
    float height = mapSize.y * scale;
    float x = mainWindow.getSize().x * 0.85;
    float y = mainWindow.getSize().y * 0.82;

    sf::RectangleShape minimapBackground({width, height});
    minimapBackground.setPosition({x, y});
    minimapBackground.setFillColor(sf::Color(36, 40, 47));
    minimapBackground.setOutlineColor(sf::Color(90, 95, 105));
    minimapBackground.setOutlineThickness(1);

    // Ici, je crée le sprite de la minimap, en lui donnant l'echelle
    sf::Sprite minimapImage(texture);
    minimapImage.setScale({scale, scale});
    minimapImage.setPosition({x, y});

    mainWindow.setView(mainWindow.getDefaultView());
    mainWindow.draw(minimapBackground);
    mainWindow.draw(minimapImage);
}