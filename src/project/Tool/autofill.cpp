#include "autofill.hpp"
#include "../map.hpp"
#include <algorithm>




AutoFill::AutoFill(std::shared_ptr<Map> map, ClientNetworkManager &manager) : Tool(map,manager){ 
  type_ = AUTOFILL;
  temporaryLayer_ = map->getTempLayer();
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

void AutoFill::generateSprites() { 
    clear(); 
    if (assets_.empty()) 
        return; 

    int minX = std::min(startPos_.x, lastPos_.x); 
    int maxX = std::max(startPos_.x, lastPos_.x); 
    int minY = std::min(startPos_.y, lastPos_.y); 
    int maxY = std::max(startPos_.y, lastPos_.y);

    srand(seed_);

    for (float j = minY; j <= maxY; ) {
        float maxRowHeight = 0;
        
        for (float i = minX; i <= maxX; ) {
            Asset* asset = assets_.at(rand() % assets_.size());
            sf::Sprite sprite = sf::Sprite(*(asset->texture));

            sf::FloatRect localBounds = sprite.getLocalBounds();
            sprite.setOrigin(sf::Vector2f(localBounds.size.x / 2.0f, localBounds.size.y / 2.0f));

            float scaleFactor = size_*getScale() / localBounds.size.x;
            sprite.setScale(sf::Vector2f(scaleFactor, scaleFactor));
            
            sprite.setRotation(sf::degrees(rotation_));

            sf::FloatRect globalBounds = sprite.getGlobalBounds();

            sprite.setPosition(sf::Vector2f(i + globalBounds.size.x / 2.0f, j + globalBounds.size.y / 2.0f));
            
            temporaryLayer_->draw(sprite, asset->id);

            i += globalBounds.size.x + spacing_;
            
            if (globalBounds.size.y > maxRowHeight) 
                maxRowHeight = globalBounds.size.y;
        }
        
        float step = (maxRowHeight > 0) ? maxRowHeight : size_;
        j += step + spacing_;
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
  size_ = size;
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
    if(temporaryLayer_->getSprites().empty())
        return;
    std::vector<sf::Vector2f> positions;
    std::vector<std::string> asset_ids;

    for(const auto& [id,spriteObj] : temporaryLayer_->getSprites()){
      positions.push_back(spriteObj.sprite.getPosition());
      asset_ids.push_back(spriteObj.assetId);
    }
    // map_->createSpriteLayer(map_->getCurrentLayer()->getId());
    // std::shared_ptr<Layer> layer = (map_->getLayers()[map_->getLayerSelected() +1]);
    // if(layer->getType() == SPRITELAYER){
    // static_pointer_cast<SpriteLayer>(layer)->draw(temporaryLayer_);
    manager_.autofill(map_->getId(),map_->getCurrentLayer()->getId(),asset_ids,positions,rotation_,size_);
    clear();
    }
  // }

void AutoFill::clear(){temporaryLayer_->getSprites().clear();}
