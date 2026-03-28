#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>
#include <SFML/Config.hpp>
#include <iostream>
#include <algorithm>
#include "map.hpp"
#include "Layer/pixellayer.hpp"
#include "Layer/spritelayer.hpp"

using namespace std;


Map::Map(uint id, sf::Vector2u size , unsigned int scale,vector<shared_ptr<Layer>> layers) :
     id_{id},size_{size},scale_{scale},layers_{std::move(layers)}, move_{static_cast<float>(size_.x), static_cast<float>(size_.y)},sprite_(render_texture_.getTexture()){
    if (!render_texture_.resize(size)){
        std::cerr<<"Error : size of the layer : ("<<size.x<<","<<size.y<< ")"<<std::endl;
    } //TODO: gérer l'erreur
    sprite_.setTexture(render_texture_.getTexture(),true);
    hasLayer() ? selected_ = layers_.size()-1 : selected_ = 0;
}

Map::Map(uint id, sf::Vector2u size , unsigned int scale) :
     id_{id},size_{size},
    scale_{scale}, move_{static_cast<float>(size_.x), static_cast<float>(size_.y)},sprite_(render_texture_.getTexture()){
    if (!render_texture_.resize(size)){} //TODO: gérer l'erreur
    sprite_.setTexture(render_texture_.getTexture(),true);
    hasLayer() ? selected_ = layers_.size()-1 : selected_ = 0;
    createPixelLayer();
}

Zoom& Map::getZoom() { return zoom_; }

AssetManager& Map::getAssetManager() { return assetmanager_; }

sf::Vector2u Map::getSize()const {return size_;}

bool Map::hasLayer()const {return !layers_.empty();}

void Map::selectLayer(unsigned int i) { selected_ = i; }

void Map::selectLayerId(uint id) {
    for (size_t i = 0; i < layers_.size(); ++i) {
        if (layers_[i]->getId() == id) {
            selectLayer(i);
            break;
        }
    }
}

unsigned int Map::getScale() const { return scale_; }

vector<shared_ptr<Layer>>& Map::getLayers() { return layers_; }


void Map::createPixelLayer(){
    int n = layers_.size() + 1;
    std::string name = "Couche Pixel (" + std::to_string(n) + ")";
    shared_ptr<PixelLayer> pixellayer = make_shared<PixelLayer>(nextLayerId_,name, size_);
    nextLayerId_ +=1;
    layers_.push_back(pixellayer);
    layers_.size() == 1 ? selected_ = 0 : selected_ += 1; // évite d'avoir des numéros de couche négative
}


void Map::createSpriteLayer(){
    int n = layers_.size() + 1;
    std::string name = "Couche Sprite (" + std::to_string(n) + ")";
    shared_ptr<SpriteLayer> spritelayer = make_shared<SpriteLayer>(nextLayerId_,name,size_); 
    nextLayerId_ +=1;
    layers_.push_back(spritelayer);
    layers_.size() == 1 ? selected_ = 0 : selected_ += 1;
}

  void Map::layerUp(){
    if (selected_ == 0) return;
    std::swap(layers_[selected_], layers_[selected_ - 1]);
    selected_ -= 1;
}
  void Map::layerDawn(){
    if (selected_ >= layers_.size()-1) return;
    std::swap(layers_[selected_], layers_[selected_ + 1]);
    selected_ += 1;
}


void Map::deleteLayer(uint layer_id){
    if (layers_.size() <= 1) return;
    int j;

    for(int i = 0; i<layers_.size(); i++){
        if(layers_[i]->getId() == layer_id){
            j = i;
            break;
        }
    }
    if(j != -1)
        layers_.erase(layers_.begin() + j);
    if (selected_ == j && selected_ > 0) selected_ -= 1;
    else if (selected_ > j) selected_ -= 1;
}

void Map::deleteLayer(){
    if (layers_.size() <= 1) return;
    layers_.erase(layers_.begin() + selected_);
    selected_ = (selected_ > 0) ? selected_ - 1 : 0;
}


shared_ptr<Layer> Map::getCurrentLayer(){
    if(layers_.size() == 0){
    return nullptr;}
    if(selected_ >= layers_.size())
        selected_ = layers_.size()-1;
    return layers_[selected_];
}

void Map::displayMap(sf::RenderWindow& window, sf::View& viewMap) {

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


    render_texture_.clear(sf::Color::White);

    for (auto& layer: layers_) {
        // We are drawing all the layers on the texture of the map
        layer->drawLayer(render_texture_);
    }
    // And then we display the texture of the map
    render_texture_.display();
    window.draw(sprite_);
}

unsigned int Map::getLayerSelected() const {
    return selected_;
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

void Map::zooming(sf::Event::MouseWheelScrolled const* event) {
    if (event->delta > 0) { // regarde si l'utilisateur veut faire un zoom avant
        zoom_.zoomIn();
    }
    else if (event->delta < 0) {  // regarde si l'utilisateur veut faire un zoom arrière
        zoom_.zoomOut();
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

uint Map::getId(){
    return id_;
}

void Map::setId(uint newId) {
    id_ = newId;
}
