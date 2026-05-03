#include "autofill.hpp"
#include "../map.hpp"
#include <algorithm>




AutoFill::AutoFill(std::shared_ptr<Map> map, ClientNetworkManager &manager) : Tool(map,manager){ 
  type_ = AUTOFILL;
  temporaryLayer_ = dynamic_pointer_cast<SpriteLayer>(map_->getLayer(0));
}





void AutoFill::onPress(sf::Vector2i pos){
  startPos_ = pos;
  lastPos_ = pos;
}

void AutoFill::onDrag(sf::Vector2i pos){
  lastPos_ = pos;
}

void AutoFill::onRelease(){
  generateSprites();
}

void AutoFill::generateSprites(){ 
  clear(); 
  if(assets_.empty()) 
    return; 
  int minX = std::min(startPos_.x, lastPos_.x); 
  int maxX = std::max(startPos_.x, lastPos_.x); 
  int minY = std::min(startPos_.y, lastPos_.y); 
  int maxY = std::max(startPos_.y, lastPos_.y);
  srand(seed_);

  float currentStepY = size_;

  for(float j = minY;j <= maxY; j+= spacing_ + currentStepY){
    float maxRowHeight = 0;
    for(float i = minX; i<= maxX;){
      Asset* asset = assets_.at(rand()%assets_.size());
      sf::Sprite sprite = sf::Sprite(*(asset->texture));

      sf::FloatRect bounds = sprite.getLocalBounds();
      sprite.setOrigin(sf::Vector2f(bounds.size.x/2.0f,bounds.size.y/2.0f));

      float spriteSizeX = size_*asset->size_m_horizontal;
      float scaleFactor = spriteSizeX/bounds.size.x;
      sprite.scale(sf::Vector2f(scaleFactor,scaleFactor));

      sprite.setRotation(sf::degrees(rotation_));
      sprite.setPosition(sf::Vector2f(i+spriteSizeX/2.0f,j));
      temporaryLayer_->draw(sprite, asset->id);


      i+= spriteSizeX + spacing_;
      float spriteSizeY = bounds.size.y * scaleFactor;
      if(spriteSizeY > maxRowHeight) 
        maxRowHeight = spriteSizeY;
    }
    currentStepY = (maxRowHeight > 0) ? maxRowHeight : size_;

  }
  srand(time(NULL));
}


void AutoFill::setSpacing(float spacing){
  spacing_ = spacing * getScale();
  generateSprites();
}
void AutoFill::setRotation(float rotation){
  rotation_ = rotation;
  generateSprites();
}
void AutoFill::setSize(float size){
  unsigned int mapScale = map_->getScale();
  size_ = size*mapScale;
  // if(size_ > std::abs(lastPos_ - startPos_)){
  //   size_ = 
  // }
  generateSprites();
}
void AutoFill::setSeed(uint seed){
  seed_ = seed;
  generateSprites();
}

void AutoFill::addAsset(const std::string &id){
  Asset *asset = map_->getAssetManager().getAsset(id);
  if (asset != nullptr)
    assets_.push_back(asset);
}

void AutoFill::removeAsset(const std::string &id){
  Asset *asset = map_->getAssetManager().getAsset(id);
  auto iterator = std::find(assets_.begin(), assets_.end(), asset);
  if (iterator != assets_.end())
    assets_.erase(iterator);
}

void AutoFill::clearAsset(){assets_.clear();}





void AutoFill::apply(){
  if(map_->getCurrentLayer()->getType() == SPRITELAYER){
    std::vector<sf::Vector2f> positions;
    std::vector<std::string> asset_ids;

    for(const auto& [id,spriteObj] : temporaryLayer_->getSprites()){
      positions.push_back(spriteObj.sprite.getPosition());
      asset_ids.push_back(spriteObj.assetId);
    }
    
    manager_.autofill(map_->getId(),map_->getCurrentLayer()->getId(),asset_ids,positions,rotation_,size_);
    static_pointer_cast<SpriteLayer>(map_->getCurrentLayer())->draw(temporaryLayer_);
    clear();
  }
}

void AutoFill::clear(){temporaryLayer_->getSprites().clear();}
