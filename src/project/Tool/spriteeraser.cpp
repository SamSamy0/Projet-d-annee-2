#include "spriteeraser.hpp"
#include "../Layer/layer.hpp"
#include "../Layer/spritelayer.hpp"
#include "../map.hpp"
#include <memory>

SpriteEraser::SpriteEraser(std::shared_ptr<Map> map,
                           ClientNetworkManager &manager)
    : Brush(map, manager) {
  type_ = SPRITEERASER;
}

void SpriteEraser::setShape(Shape s) { shape_ = s; }
Shape SpriteEraser::getShape() const{return shape_;}

bool SpriteEraser::checkColision(sf::Vector2i pos, sf::FloatRect r) {
  // border of the sprite
  int up = r.position.y;
  int left = r.position.x;
  int down = r.position.y + r.size.y;
  int right = r.position.x + r.size.x;

  // closest point of the sprite from the mouse
  float closest_y = std::max(up, std::min(pos.y, down));
  float closest_x = std::max(left, std::min(pos.x, right));

  // distance between the mouse and the sprite
  float dx = std::abs(pos.x - closest_x);
  float dy = std::abs(pos.y - closest_y);

  switch (shape_) {
  case SQUARE: {

    return dx <= size_m_.x * getScale() / 2 && dy <= size_m_.x * getScale() / 2;
    break;
  }
  case CIRCLE: {
    return (dx * dx) + (dy * dy) <=
           (size_m_.x * getScale() / 2) * (size_m_.x * getScale() / 2);
    break;
  }

  case DIAMOND: {
    return (dx / (size_m_.x * getScale() / 2)) +
               (dy / (size_m_.y * getScale() / 2)) <=
           1.0f;

    break;
  }
  }
  return false;
}

int SpriteEraser::paint(sf::Vector2i pos) {
  std::shared_ptr<Layer> layer = map_->getCurrentLayer();
  if (layer->getType() != SPRITELAYER)
    return -1;

  std::shared_ptr<SpriteLayer> spritelayer =
      static_pointer_cast<SpriteLayer>(layer);

  if (!spritelayer)
    return -1;

  /*I had two choices : use the id or the index of the sprite
   * i chosed to use the id because its cleaner more logical even if complexity
   * is higher */
  const std::unordered_map<uint, SpriteObject> &sprites = spritelayer->getSprites();
  pos -= spritelayer->getOffset();

  for (const auto &[id, spriteObj] : sprites) {
    if (checkColision(pos, spriteObj.sprite.getGlobalBounds())) {
      uint spriteId = spriteObj.id;
      std::cout << spriteId << " id du sprite effacé" << std::endl;
      spritelayer->erase(spriteId);
      return spriteId;
    }
  }
  return -1;
}

void SpriteEraser::paintSender(sf::Vector2i pos) {
  int id = paint(pos);
  if(id != -1){
    manager_.eraseSprite(map_->getId(), map_->getCurrentLayer()->getId(),id);
}
}
