#pragma once
#include "../assetmanager.hpp"
#include "../../client/clientnetwork.hpp"
#include <memory>
#include "brush.hpp"

class SpriteBrush : public Brush {
  std::vector<Asset *> assets_;
  float offset_ = 0;
  Shape shape_ = SQUARE;

public:
  SpriteBrush(std::shared_ptr<Map> map, ClientNetworkManager &manager);
  Asset *getAsset();
  void paint(sf::Vector2i pos) override;
  void setOffset(float offset);
  void setShape(Shape s);
  void setSize(float x, float y) override;
  void addAsset(const std::string &id);
  void removeAsset(const std::string &id);
  void paintSender(sf::Vector2i pos) override;
  void clearAsset();
  float getOffset();
};
