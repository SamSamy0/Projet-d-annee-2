#pragma once
#include "brush.hpp"
#include "../assetmanager.hpp"
#include "../../client/clientnetwork.hpp"
#include <memory>

class SpriteBrush : public Brush {
  std::vector<Asset*> assets_;
  float offset_ = 0;
  Shape shape_ = SQUARE;

public:
  SpriteBrush(std::shared_ptr<Map> map, ClientNetworkManager& manager);
  Asset* getAsset();
  void paint(sf::Vector2i pos) override;
  void setOffset(float offset);
  void setShape(Shape s);
  void setSize(float x, float y =1)override;
  void addAsset(const std::string& id);
  void removeAsset(const std::string& id);
  void clearAsset();

};
