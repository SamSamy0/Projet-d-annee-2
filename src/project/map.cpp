#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include "map.hpp"
#include "layer.hpp"
#include <SFML/Config.hpp>
#include <iostream>

using namespace std;

Map::Map(int mapId, sf::Vector2u size , unsigned int scale,vector<shared_ptr<Layer>> layers) : 
    id_{mapId}, size_{size},scale_{scale},layers_{std::move(layers)}, move_{static_cast<float>(size_.x), static_cast<float>(size_.y)}{
    hasLayer() ? selected_ = layers_.size()-1 : selected_ = 0;
}


Map::Map(int mapId, sf::Vector2u size , unsigned int scale) : 
    id_{mapId}, size_{size},
    scale_{scale}, move_{static_cast<float>(size_.x), static_cast<float>(size_.y)}{
    hasLayer() ? selected_ = layers_.size()-1 : selected_ = 0;
    createPixelLayer();
}


sf::Vector2u Map::getSize()const {return size_;}

bool Map::hasLayer()const {return !layers_.empty();}

unsigned int Map::getScale() const { return scale_; }

vector<shared_ptr<Layer>>& Map::getLayers() { return layers_; }

void Map::insertLayer(shared_ptr<Layer> layer) { layers_.insert(layers_.begin()+selected_+1, layer) ; }

void Map::createPixelLayer(){

    shared_ptr<PixelLayer> pixellayer = make_shared<PixelLayer>("Nouvelle Couche",size_);
    layers_.push_back(pixellayer);
    layers_.size() ==1? selected_ = 0: selected_+=1;
}

shared_ptr<Layer> Map::getCurrentLayer(){
    if(layers_.size() == 0){
    return nullptr;}
    else{return layers_[selected_];}
}

void Map::displayMap(sf::RenderWindow& window, sf::View& viewMap)

{
   
    // On crée la map => zone dessinable
    sf::RectangleShape map;
    map.setSize(sf::Vector2f(size_.x, size_.y));
    map.setOrigin(sf::Vector2f((float)(size_.x) / 2, (float)(size_.y) / 2));
    map.setPosition(sf::Vector2f((float)(size_.x) / 2, (float)(size_.y) / 2));
    map.setFillColor(sf::Color::White);
    
    // Pour éviter que SFML n'étire la carte si celle-ci n'a pas la meme taille que la fenêtre, on utilise un système de ratio
    // Note : idée proposée par ChatGPT !!
    float windowRatio = static_cast<float>(window.getSize().x) / window.getSize().y;
    float mapRatio = (float)(size_.x) / (float)(size_.y);
    float zoomFactor = zoom_.getZoom();

    if (windowRatio > mapRatio) {
        float newWidth = size_.y * windowRatio;
        viewMap.setSize(sf::Vector2f(newWidth * zoomFactor, size_.y * zoomFactor));
    } else {
        float newHeight = size_.x / windowRatio;
        viewMap.setSize(sf::Vector2f(size_.x * zoomFactor, newHeight * zoomFactor));
    }
    // Fin de l'idée de ChatGPT
    
    viewMap.setCenter(sf::Vector2f(move_.positionX_ + (float)(size_.x) / 2, move_.positionY_ + (float)(size_.y) / 2));

    // On règle la fenêtre et on l'affiche
   
    window.draw(map);
    for (auto& layer: layers_) {
        layer->drawLayer(window);
    }
}

void Map::detectZooming(sf::Event event) {
    if (const auto* mouse = event.getIf<sf::Event::MouseWheelScrolled>()) {
        if (mouse->delta > 0) { // regarde si l'utilisateur veut faire un zoom avant
            zoom_.zoomIn(); 
        }
        else if (mouse->delta < 0) {  // regarde si l'utilisateur veut faire un zoom arrière
            zoom_.zoomOut();
        }
    }  
}

void Map::detectMovement()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))) { // regarde si l'utilisateur veut aller à gauche
        move_.goLeft(zoom_.getZoom());
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))) { // regarde si l'utilisateur veut aller à droite
        move_.goRight(zoom_.getZoom());
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))) { // regarde si l'utilisateur veut aller en haut
        move_.goTop(zoom_.getZoom());
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))) { // regarde si l'utilisateur veut aller en bas
        move_.goDown(zoom_.getZoom());
    }
}
