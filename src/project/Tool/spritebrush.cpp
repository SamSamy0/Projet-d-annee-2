#include "spritebrush.hpp"
#include "../Layer/spritelayer.hpp"
#include "../map.hpp"
#include <memory>
#include <algorithm>


SpriteBrush::SpriteBrush(std::shared_ptr<Map> map, ClientNetworkManager &manager) : Brush(map, manager){
  spacing_ = size_m_.x * getScale();
  if (spacing_ < 1)
    spacing_ = 1.0f;
  type_ = SPRITEBRUSH;
}

void SpriteBrush::setOffset(float offset){
  offset_ = offset;
  spacing_ = (size_m_.x + offset_) * getScale();
                      
  if (spacing_ < 1)
    spacing_ = 1.0f;
}
void SpriteBrush::setShape(Shape s){shape_ = s;}

void SpriteBrush::setSize(float x, float y) {
  //
  /* change the size and change the spacing in function of it */
  Brush::setSize(x, y);
  spacing_ =(size_m_.x + offset_) * getScale();
                      
  if (spacing_ < 1)
    spacing_ = 1.0f;
}

void SpriteBrush::addAsset(const std::string &id) {
  Asset *asset = map_->getAssetManager().getAsset(id);
  if (asset != nullptr)
    assets_.push_back(asset);
}
void SpriteBrush::removeAsset(const std::string &id) {
  Asset *asset = map_->getAssetManager().getAsset(id);
  auto iterator = std::find(assets_.begin(), assets_.end(), asset);
  if (iterator != assets_.end())
    assets_.erase(iterator);
}

void SpriteBrush::clearAsset() { assets_.clear(); }

Asset *SpriteBrush::getAsset() {
  /*Return a randow selected Asset */
  return assets_[rand() % assets_.size()];
}


void SpriteBrush::paint(sf::Vector2i pos,Asset* asset){
  std::shared_ptr<Layer> layer = map_->getCurrentLayer();
  if (layer->getType() == SPRITELAYER) {
    std::shared_ptr<SpriteLayer> spritelayer =
        static_pointer_cast<SpriteLayer>(layer);
    sf::Sprite sprite = sf::Sprite(*(asset->texture));
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(sf::Vector2f(bounds.size.x / 2, bounds.size.y / 2));

    float scale = size_m_.x * getScale() / bounds.size.x;
    sprite.setScale(sf::Vector2f(scale, scale));

    sf::Vector2f offset =
        sf::Vector2f(spritelayer->getOffset().x, spritelayer->getOffset().y);
    sprite.setPosition(sf::Vector2f(static_cast<float>(pos.x) - offset.x,
                                    static_cast<float>(pos.y) - offset.y));

    spritelayer->draw(sprite);
  }
}


  void SpriteBrush::paintSender(sf::Vector2i pos){
  if(assets_.empty())
    return;
  Asset* asset = getAsset();
  paint(pos,asset);
  manager_.drawSprite(map_->getId(),map_->getCurrentLayer()->getId(),asset->id,pos.x,pos.y,size_m_.x);
}
