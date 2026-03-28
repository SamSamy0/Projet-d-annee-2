#pragma once
#include "assetmanager.hpp"
#include "move.hpp"
#include "zoom.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// TODO: DEFINIR DES VALEURS MAX ET MIN POUR LA SIZE, LA SCALE

using namespace std;

class Layer;

class Map {
  uint nextLayerId_ = 0;
  uint id_;
  sf::Vector2u size_;
  unsigned int scale_;
  vector<shared_ptr<Layer>> layers_;
  sf::RenderTexture render_texture_;
  sf::Sprite sprite_;
  Zoom zoom_;
  Move move_;
  unsigned int selected_;
  AssetManager assetmanager_;

public:
  Map(uint id,sf::Vector2u size, unsigned int scale);
  Map(uint id,sf::Vector2u size, unsigned int scale,vector<shared_ptr<Layer>> layers);
  sf::Vector2u getSize() const;
  unsigned int getScale() const;
  Zoom &getZoom();
  AssetManager &getAssetManager();
  vector<shared_ptr<Layer>> &getLayers();
  shared_ptr<Layer> getCurrentLayer();
  unsigned int getLayerSelected() const;
  uint getId();
  void selectLayer(unsigned int i);
  void selectLayerId(uint id);
  bool hasLayer() const;
  const sf::RenderTexture& getRenderTexture() const { return render_texture_; }
  void createPixelLayer();
  void createSpriteLayer();
  void layerUp();
  void layerDawn();
  void deleteLayer();
  void deleteLayer(uint layer_id);
  void displayMap(sf::RenderWindow &window, sf::View &viewMap);
  void closeWindowManager(sf::RenderWindow &window, const sf::Event &ev);
  void detectMovement();
  void zooming(sf::Event::MouseWheelScrolled const *event);
  // bool isInside(sf::Vector2i pos)const; //WARNING: ne sert à rien pour
  // l'instant
  void detectZooming(sf::Event event);
  void setId(uint newId);
};
