#pragma once
#include <string>
#include <unordered_set>
#include <SFML/Graphics.hpp>
#include <map>

class SpriteManager {
  std::unordered_set<std::string> assetsId_;
  uint nextId_ = 0;
  std::map<uint, sf::Texture> importedSprite_;

  public:
  SpriteManager(std::map<uint, sf::Texture> importedSprite);
  SpriteManager();
  void loadFromJson();
  uint addSprite(const sf::Texture& texture);
  std::map<uint, sf::Texture>& getImportedSprites();

  bool verifyAssetId(const std::string& assetId);
};