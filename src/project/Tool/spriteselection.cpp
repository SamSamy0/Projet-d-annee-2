#include "spriteselection.hpp"
#include "../Layer/layer.hpp"
#include "../Layer/spritelayer.hpp"
#include "../map.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

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
    const std::unordered_map<uint, SpriteObject> &sprites = spritelayer->getSprites();
    for (const auto &[id, spriteObj] : sprites) {
      if (spriteObj.sprite.getGlobalBounds().contains(
              sf::Vector2f(pos.x, pos.y))) {
        hitedId = spriteObj.id;
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
      shift(pos);
    }

    else if (state_ == SelectionState::ROTATING) {
      rotate(pos);

    } else if (state_ == SelectionState::RESIZING) {
      resize(pos);
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

      const std::unordered_map<uint, SpriteObject> &sprites = spritelayer->getSprites();

      bool isCtrl = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);

      if (!isCtrl) {
        selected_.clear();
      }
      for (const auto &[id, spriteObj] : sprites) {
        if (selectionRect.findIntersection(spriteObj.sprite.getGlobalBounds())) {
          if (isCtrl) {
            if (!isSelected(spriteObj.id))
              selected_.push_back(
                  spriteObj
                      .id); // WARNING: ATTENTION AU COMPORTEMENT SI ON PRESSE
                            // CTRL ON CLICK PUIS ON LE RELACHE AVANT ON RELEASE
            else {
              selected_.erase(
                  std::remove(selected_.begin(), selected_.end(), spriteObj.id),
                  selected_.end());
            }
          } else
            selected_.push_back(spriteObj.id);
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
  int size = selected_.size();
  if(size == 0)
    return res;
  const std::shared_ptr<Layer> layer = map_->getCurrentLayer();

  // if (layer->getType() == SPRITELAYER) {
  //   const std::vector<SpriteObject> &sprites =
  //       static_pointer_cast<SpriteLayer>(layer)->getSprites();
  //
  //   for (const SpriteObject &sprite : sprites) {
  //     if (isSelected(sprite.id)) {
  //       res += sprite.sprite.getPosition();
  //       count++;
  //     }
  //   }
  // }
  if(layer->getType() == SPRITELAYER){
    const std::shared_ptr<SpriteLayer> spritelayer = static_pointer_cast<SpriteLayer>(layer);
    for(const uint id : selected_){
      res+= spritelayer->getSprite(id).sprite.getPosition();
    }
  }
  res.x /= size;
  res.y /= size;
  return res;
}

void SpriteSelection::rotate(sf::Vector2i pos) {
  std::shared_ptr<Layer> layer = map_->getCurrentLayer();
  if (layer->getType() == SPRITELAYER) {
    std::shared_ptr<SpriteLayer> spritelayer =
        static_pointer_cast<SpriteLayer>(layer);

    sf::Vector2f delta =
        sf::Vector2f((pos.x - lastPos_.x), (pos.y - lastPos_.y));

    float sensitivity = 0.01f;
    float scaled_dist = (delta.x - delta.y) * sensitivity;
if (scaled_dist != 0.0f) {
  std::vector<sf::Vector2f> newPositions;
  newPositions.reserve(selected_.size());
  for (uint id : selected_) {
    sf::Sprite sprite = spritelayer->getSprite(id).sprite;
    sf::Vector2f spritePos = sprite.getPosition();
    sf::Vector2f dist = spritePos - pivotPos_;

    float cosA = std::cos(scaled_dist);
    float sinA = std::sin(scaled_dist);

    // Rotation Matrix
    float newX = dist.x * cosA - dist.y * sinA;
    float newY = dist.x * sinA + dist.y * cosA;
    sf::Vector2f newPos =
        sf::Vector2f(newX + pivotPos_.x, newY + pivotPos_.y);
    spritelayer->rotateSprite(id, scaled_dist, newPos);
    newPositions.push_back(newPos);
  }
  if (!selected_.empty()) {
    manager_.rotateSprites(map_->getId(), map_->getCurrentLayer()->getId(),
                           selected_, scaled_dist, newPositions);
  }
}
          }
        }

void SpriteSelection::resize(sf::Vector2i pos) {
  std::shared_ptr<Layer> layer = map_->getCurrentLayer();
  if (layer->getType() == SPRITELAYER) {
    std::shared_ptr<SpriteLayer> spritelayer =
        static_pointer_cast<SpriteLayer>(layer);

    sf::Vector2f delta =
        sf::Vector2f((pos.x - lastPos_.x), (pos.y - lastPos_.y));

    float sensitivity = 0.01f;
    float scaleFactor = 1.0f + (delta.x - delta.y) * sensitivity;

    if (scaleFactor > 0.001) {
      std::vector<sf::Vector2f> newPositions;
      newPositions.reserve(selected_.size());
      for (uint id : selected_) {
        sf::Sprite sprite = spritelayer->getSprite(id).sprite;
        sf::Vector2f spritePos = sprite.getPosition();
        sf::Vector2f dist = spritePos - pivotPos_;
        sf::Vector2f newPos = pivotPos_ + dist * scaleFactor;
        spritelayer->resizeSprite(id, newPos, scaleFactor);
        newPositions.push_back(newPos);
      }
      if (!selected_.empty()) {
        manager_.resizeSprites(map_->getId(), map_->getCurrentLayer()->getId(),
                               selected_, newPositions, scaleFactor);
      }
    }
      }
    }
void SpriteSelection::shift(sf::Vector2i pos) {
  std::shared_ptr<Layer> layer = map_->getCurrentLayer();
  if (layer->getType() == SPRITELAYER) {
    std::shared_ptr<SpriteLayer> spritelayer =
        static_pointer_cast<SpriteLayer>(layer);

    sf::Vector2i delta = sf::Vector2i(pos.x - lastPos_.x, pos.y - lastPos_.y);

    for (uint id : selected_) {
      spritelayer->shiftSprite(id, delta);
    }
    if (!selected_.empty()) {
      manager_.moveSprites(map_->getId(), map_->getCurrentLayer()->getId(),
                           selected_, delta);
    }
  }
}
