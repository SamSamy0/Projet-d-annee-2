#include "spriteselection.hpp"
#include "../Layer/layer.hpp"
#include "../Layer/spritelayer.hpp"
#include "../map.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <cmath>
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
    startSelectionPos_ = pos;

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
          selected_.erase(
              std::remove(selected_.begin(), selected_.end(), hitedId),
              selected_.end()); // remove the id from selection
        } else {
          selected_.push_back(hitedId);
        }
      } else {
        if (!isSelected(hitedId)) {
          selected_.clear();
          selected_.push_back(hitedId);
        }
      }
      state_ = SelectionState::DRAGING;
      pivotPos_ = findPivot();
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt)) {
        state_ = SelectionState::ROTATING;

      } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
        state_ = SelectionState::RESIZING;
      }
    } else {
      if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) {
        selected_.clear();
      }
      state_ = SelectionState::SELECTION;
    }
  }
  lastPos_ = pos;
}

void SpriteSelection::onDrag(sf::Vector2i pos) {
  std::shared_ptr<Layer> layer = map_->getCurrentLayer();
  if (layer->getType() == SPRITELAYER) {
    std::shared_ptr<SpriteLayer> spritelayer =
        static_pointer_cast<SpriteLayer>(layer);
    pos -= spritelayer->getOffset();
    sf::Vector2i delta = sf::Vector2i(pos.x - lastPos_.x, pos.y - lastPos_.y);

    if (state_ == SelectionState::DRAGING) {
      for (uint id : selected_) {
        spritelayer->shiftSprite(id, delta);
      }
    }

    else if (state_ == SelectionState::ROTATING) {
      sf::Vector2f delta =
          sf::Vector2f((pos.x - lastPos_.x), (pos.y - lastPos_.y));
      float sensitivity = 0.01f;
      float scaled_dist = (delta.x - delta.y) * sensitivity;

      if (scaled_dist != 0.0f) {
        for (uint id : selected_) {
          spritelayer->rotateSprite(id, scaled_dist, pivotPos_);
        }
      }

    } else if (state_ == SelectionState::RESIZING) {
      sf::Vector2f delta =
          sf::Vector2f((pos.x - lastPos_.x), (pos.y - lastPos_.y));
      float sensitivity = 0.01f;
      float scaleFactor = 1.0f + (delta.x - delta.y) * sensitivity;

      if (scaleFactor > 0.001) {
        for (uint id : selected_) {
          spritelayer->resizeSprite(id, scaleFactor, pivotPos_);
        }
      }
    }
    lastPos_ = pos;
  }
}

void SpriteSelection::onRelease() {
  std::shared_ptr<Layer> layer = map_->getCurrentLayer();
  if (layer->getType() == SPRITELAYER) {
    std::shared_ptr<SpriteLayer> spritelayer =
        static_pointer_cast<SpriteLayer>(layer);

    if (state_ == SelectionState::SELECTION) {
      sf::FloatRect selectionRect;

      selectionRect.size.x = std::abs(startSelectionPos_.x - lastPos_.x);
      selectionRect.size.y = std::abs(startSelectionPos_.y - lastPos_.y);

      // top left corner of the Rectangle
      selectionRect.position.x = std::min(startSelectionPos_.x, lastPos_.x);
      selectionRect.position.y = std::min(startSelectionPos_.y, lastPos_.y);

      const std::vector<SpriteObject> &sprites = spritelayer->getSprites();

      bool isCtrl = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);

      if (!isCtrl) {
        selected_.clear();
      }
      for (const auto &sprite : sprites) {
        if (selectionRect.findIntersection(sprite.sprite.getGlobalBounds())) {
          if (isCtrl) {
            if (!isSelected(sprite.id))
              selected_.push_back(
                  sprite
                      .id); // WARNING: ATTENTION AU COMPORTEMENT SI ON PRESSE
                            // CTRL ON CLICK PUIS ON LE RELACHE AVANT ON RELEASE
            else {
              selected_.erase(
                  std::remove(selected_.begin(), selected_.end(), sprite.id),
                  selected_.end());
            }
          } else
            selected_.push_back(sprite.id);
        }
      }
    }

    else if (state_ == SelectionState::DRAGING) {
    }
    isDrawing_ = false;
  }
  state_ = SelectionState::NONE;
}

sf::Vector2f SpriteSelection::findPivot() {
  sf::Vector2f res = sf::Vector2f(0, 0);
  std::vector<SpriteObject> sprites;

  if (map_->getCurrentLayer()->getType() == SPRITELAYER)
    sprites =
        static_pointer_cast<SpriteLayer>(map_->getCurrentLayer())->getSprites();

  for (SpriteObject &sprite : sprites) {
    for (uint id : selected_) {
      if (sprite.id == id)
        res += sprite.sprite.getPosition();
    }
  }
  res.x /= selected_.size();
  res.y /= selected_.size();

  return res;
}
