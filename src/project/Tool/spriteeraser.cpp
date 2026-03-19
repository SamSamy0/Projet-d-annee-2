#include "spriteeraser.hpp"
#include "../Layer/layer.hpp"
#include "../Layer/spritelayer.hpp"
#include "../map.hpp"
#include <memory>

SpriteEraser::SpriteEraser(std::shared_ptr<Map> map, ClientNetworkManager &manager) : Brush(map, manager) {
  type_ = SPRITEERASER;
}

void SpriteEraser::setShape(Shape s) { shape_ = s; }

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
    return (dx * dx) + (dy * dy) <= (size_m_.x * getScale() / 2) * (size_m_.x * getScale() / 2);
    break;
  }

  case DIAMOND: {
    return (dx / (size_m_.x * getScale() / 2)) + (dy / (size_m_.y * getScale() / 2)) <= 1.0f;

    break;
  }
  }
}

void SpriteEraser::paint(sf::Vector2i pos) {
  std::shared_ptr<Layer> layer = map_->getCurrentLayer();
  if (layer->getType() != SPRITELAYER)
    return;

  std::shared_ptr<SpriteLayer> spritelayer = static_pointer_cast<SpriteLayer>(layer);

  if (!spritelayer)
    return;

  /*I had two choices : use the id or the index of the sprite
   * i chosed to use the id because its cleaner more logical even if complexity is higher */
  const std::vector<SpriteObject> &sprites = spritelayer->getSprites();
  pos -= spritelayer->getOffset();

  for (int i = sprites.size() - 1; i >= 0; i--) {
    if (checkColision(pos, sprites[i].sprite.getGlobalBounds())) {
      spritelayer->erase(sprites[i].id);
      break;
    }
  }
}



  void SpriteEraser::paintSender(sf::Vector2i pos){paint(pos);} //Elle n'envoie pas encore de message 
