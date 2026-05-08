#include "../Application.hpp"
#include "GameView.hpp"

void GameView::initMinimap() {
}

void GameView::drawMinimap() {
    auto& mainWindow = app_.getWindow();
    auto map = project->getMap();
    auto& texture = map->getRenderTexture().getTexture();
    sf::Vector2u mapSize = map->getSize();
    float scale = ((200.0 / mapSize.x) < (150.0 / mapSize.y)) ? (200.0 / mapSize.x) : (150.0 / mapSize.y);

    // Je crée la bordure grise à la minimap
    sf::RectangleShape minimapBackground({mapSize.x * scale, mapSize.y * scale});
    minimapBackground.setPosition({static_cast<float>(mainWindow.getSize().x) - mapSize.x * scale - 10.f, static_cast<float>(mainWindow.getSize().y) - mapSize.y * scale - 10.f});
    minimapBackground.setFillColor(sf::Color(36, 40, 47));
    minimapBackground.setOutlineColor(sf::Color(90, 95, 105));
    minimapBackground.setOutlineThickness(1);

    // C'est l'image de la minimap
    sf::Sprite minimapImage(texture);
    minimapImage.setScale({scale, scale});
    minimapImage.setPosition(minimapBackground.getPosition());

    // A partir d'ici, je m'occupe de la vue de l'utilisateur sur la minimap
    sf::View view = project->getView();
    sf::Vector2f minimapOrigin = minimapBackground.getPosition();
    float minimapWidth = mapSize.x * scale;
    float minimapHeight = mapSize.y * scale;
    float visionWidth = view.getSize().x * scale;
    float visionHeight = view.getSize().y * scale;

    // Je vérifie que la taille de la vision utilisateur soit pas plus grande que la minimap
    if (visionWidth > minimapWidth) visionWidth = minimapWidth;
    if (visionHeight > minimapHeight) visionHeight = minimapHeight;

    float positionX = minimapOrigin.x + (view.getCenter().x - view.getSize().x / 2.f) * scale;
    float positionY = minimapOrigin.y + (view.getCenter().y - view.getSize().y / 2.f) * scale;
    
    // Je regarde si la position est bien dans la minimap
    float minX = minimapOrigin.x;
    float minY = minimapOrigin.y;
    float maxX = minimapOrigin.x + minimapWidth;
    float maxY = minimapOrigin.y + minimapHeight;

    if (positionX < minX) {
        positionX = minX;
    } else if (positionX + visionWidth > maxX) {
        positionX = maxX - visionWidth;
    }

    if (positionY < minY) {
        positionY = minY;
    } else if (positionY + visionHeight > maxY) {
        positionY = maxY - visionHeight;
    }

    // Ici je construit le rectangle bleu qui affiche ce que l'utilisateur voit dans la minimap
    sf::RectangleShape userVision({visionWidth, visionHeight});
    userVision.setPosition({positionX, positionY});
    userVision.setFillColor(sf::Color(0, 0, 128, 50));
    userVision.setOutlineColor(sf::Color(0, 0, 128));
    userVision.setOutlineThickness(1);

    // J'affiche tout
    sf::View minimapView(sf::FloatRect({0.f, 0.f}, sf::Vector2f(mainWindow.getSize())));
    mainWindow.setView(minimapView);
    mainWindow.draw(minimapBackground);
    mainWindow.draw(minimapImage);
    mainWindow.draw(userVision);
}
