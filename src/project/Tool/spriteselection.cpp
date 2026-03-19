#include "spriteselection.hpp"
#include "../Layer/layer.hpp"
#include "../Layer/spritelayer.hpp"
#include "../map.hpp"
#include <algorithm>
#include <memory>

SpriteSelection::SpriteSelection(std::shared_ptr<Map> map,
                                 ClientNetworkManager &manager)
    : Tool(map, manager) {
  type_ = SPRITESELECTION;
}

std::vector<uint> &SpriteSelection::getSelection() { return selected_; }

const SelectionState SpriteSelection::getState() const { return state_; }

bool SpriteSelection::isSelected(uint id) const {
  if (std::find(selected_.begin(), selected_.end(), id) != selected_.end()) {
    return true;
  } else {
    return false;
  }
}

void SpriteSelection::clearSelection() { selected_.clear(); }

void SpriteSelection::onPress(sf::Vector2i pos) {
  std::shared_ptr<Layer> layer = map_->getCurrentLayer();
  if (layer->getType() == SPRITELAYER) {
    std::shared_ptr<SpriteLayer> spritelayer =
        static_pointer_cast<SpriteLayer>(layer);
    pos -= spritelayer->getOffset();

    bool hit = false;
    uint hitedId;
    const std::vector<SpriteObject> &sprites = spritelayer->getSprites();
    for (int i = sprites.size() - 1; i >= 0; i--) {
      if (sprites[i].sprite.getGlobalBounds().contains(
              sf::Vector2f(pos.x, pos.y))) {
        hitedId = sprites[i].id;
        hit = true;
        break;
      }
    }

    if (hit) {
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
        if (isSelected(hitedId)) {
          // selected_.erase(hitedId);
        } else {
          selected_.push_back(hitedId);
        }
      } else {
        if (!isSelected(hitedId)) {
        }
      }
    } else {
    }
  }
}
