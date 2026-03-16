#include "spritebrush.hpp"
#include "../map.hpp"
#include "../Layer/spritelayer.hpp"
#include <memory>



SpriteBrush::SpriteBrush(std::shared_ptr<Map> map) : Brush(map){
  spacing_ = std::min(size_m_.x/1.0f * getScale(), size_m_.y/1.0f * getScale());
  if (spacing_ < 1)
    spacing_ = 1.0f;
}

void SpriteBrush::setOffset(float offset){offset_ = offset;}

void SpriteBrush::setSize(float x, float y = 1) { //WARNING: la valeur par défaut je suis pas sur
  //
  /* change the size and change the spacing in function of it */
  Brush::setSize(x,y);
  spacing_ = std::min((size_m_.x*2.0 + offset_) * getScale(), (size_m_.y*2.0+offset_) * getScale());
  if (spacing_ < 1)
    spacing_ = 1.0f;
}



void SpriteBrush::addAsset(const std::string& id){
  Asset* asset = map_->getAssetManager().getAsset(id);
  if(asset != nullptr)
    assets_.push_back(asset);
}
void SpriteBrush::removeAsset(const std::string& id){
  Asset* asset = map_->getAssetManager().getAsset(id);
  auto iterator = std::find(assets_.begin(),assets_.end(),asset);
  if(iterator != assets_.end())
    assets_.erase(iterator);
}


void SpriteBrush::clearAsset(){assets_.clear();}

Asset* SpriteBrush::getAsset(){
  /*Return a randow selected Asset */
  return assets_[rand()%assets_.size()];
}

void SpriteBrush::paint(sf::Vector2i pos){
  /*draw a random sprite which was selected on the current layer*/
  std::shared_ptr<Layer> layer = map_->getCurrentLayer();
  if(layer->getType() == SPRITELAYER){
    std::shared_ptr<SpriteLayer> spritelayer = static_pointer_cast<SpriteLayer>(layer);
    if (assets_.empty() == true)
      return;

    Asset* asset = getAsset();
    sf::Sprite sprite = sf::Sprite(*(asset->texture));
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(sf::Vector2f(bounds.size.x/2,bounds.size.y/2));

    float scale = size_m_.x*getScale()/bounds.size.x;
    sprite.setScale(sf::Vector2f(scale,scale));


    sf::Vector2f offset = sf::Vector2f(spritelayer->getOffset().x,spritelayer->getOffset().y);
    sprite.setPosition(sf::Vector2f(static_cast<float>(pos.x)-offset.x,static_cast<float>(pos.y)-offset.y));

    spritelayer->draw(sprite);
  }
}
