#pragma once
#include "tool.hpp"
#include "../Layer/spritelayer.hpp"
#include "../assetmanager.hpp"


class AutoFill : public Tool{

  std::vector<Asset *> assets_;
  sf::Vector2i startPos_;
  float spacing_ = 1.0f;
  float rotation_ = 0.0f; // Angle in degree
  float size_ = 1.0f;
  uint seed_ = 0;



  std::shared_ptr<SpriteLayer> temporaryLayer_;


  void generateSprites();


public:
  AutoFill(std::shared_ptr<Map> map, ClientNetworkManager &manager);
  void onPress(sf::Vector2i pos) override;
  void onDrag(sf::Vector2i pos) override;
  void onRelease() override;


  void setSpacing(float spacing);
  void setRotation(float rotation);
  void setSize(float size);
  void setSeed(uint seed);


  void addAsset(const std::string &id);
  void removeAsset(const std::string &id);
  void clearAsset();


  void apply();
  void clear();
};
