#pragma once
#include "../assetmanager.hpp"
#include "brush.hpp"
#include <memory>

class SpriteBrush : public Brush {
  std::vector<Asset *> assets_;
  float offset_ = 0;

public:
  SpriteBrush(std::shared_ptr<Map> map);
  Asset *getAsset();
  void paintSender(sf::Vector2i pos) override;
  void paint(sf::Vector2i pos) override;
  void setOffset(float offset);
  void setSize(float x, float y) override;
  void addAsset(const std::string &id);
  void removeAsset(const std::string &id);
  void clearAsset();
};
