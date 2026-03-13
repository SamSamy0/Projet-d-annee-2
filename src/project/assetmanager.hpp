#pragma once
#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>


struct Asset{
  std::string id;
  std::string filename;
  std::string category;
  std::string name;
  float size_m_horizontal;


  std::unique_ptr<sf::Texture> texture;
};

class AssetManager{

  std::unordered_map<std::string, Asset > assets_;





public:
  AssetManager() = default;
  void loadFromJson();
  std::shared_ptr<Asset> getAsset(const std::string& id);




public:
};

