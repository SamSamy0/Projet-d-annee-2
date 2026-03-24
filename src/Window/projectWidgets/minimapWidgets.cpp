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

    sf::RectangleShape minimapBackground({mapSize.x * scale, mapSize.y * scale});
    minimapBackground.setPosition({mainWindow.getSize().x * 0.83f, mainWindow.getSize().y * 0.76f});
    minimapBackground.setFillColor(sf::Color(36, 40, 47));
    minimapBackground.setOutlineColor(sf::Color(90, 95, 105));
    minimapBackground.setOutlineThickness(1);

    sf::Sprite minimapImage(texture);
    minimapImage.setScale({scale, scale});
    minimapImage.setPosition({mainWindow.getSize().x * 0.83f, mainWindow.getSize().y * 0.76f});

    sf::RectangleShape zoomZone({});
    auto position = project->getMap()->getPosition().getPosition();
    float zoom = project->getMap()->getZoom().getZoom();
    sf::Vector2f viewSize = {
    mainWindow.getSize().x / zoom,
    mainWindow.getSize().y / zoom
    };

    zoomZone.setSize({viewSize.x * scale, viewSize.y * scale});
    zoomZone.setPosition({
    (position.x - viewSize.x / 2.f) * scale + minimapBackground.getPosition().x,
    (position.y - viewSize.y / 2.f) * scale + minimapBackground.getPosition().y
    });
    zoomZone.setFillColor(sf::Color(0, 0, 255, 20));
    zoomZone.setOutlineColor(sf::Color(0, 0, 255));
    zoomZone.setOutlineThickness(2);

    mainWindow.setView(mainWindow.getDefaultView());
    mainWindow.draw(minimapBackground);
    mainWindow.draw(minimapImage);
    mainWindow.draw(zoomZone);
}
