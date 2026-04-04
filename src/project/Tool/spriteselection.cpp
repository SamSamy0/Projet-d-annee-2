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
    startSelectionPos_ = pos;
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
          selected_.erase(std::remove(selected_.begin(),selected_.end(),hitedId),selected_.end()); //remove the id from selection
        } else {
          selected_.push_back(hitedId);
        }
      } else {
        if (!isSelected(hitedId)) {
          selected_.clear();
          selected_.push_back(hitedId);
        }
      }
      state_ = DRAGING;
    } else {
      if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)){
        selected_.clear();
      }
      state_ = SELECTION;
    }
  }
  lastPos_ = pos;
}

void SpriteSelection::onDrag(sf::Vector2i pos) {
  std::shared_ptr<Layer> layer = map_->getCurrentLayer();
  if (layer->getType() == SPRITELAYER) {
    std::shared_ptr<SpriteLayer> spritelayer = static_pointer_cast<SpriteLayer>(layer);
    pos -= spritelayer->getOffset();
    sf::Vector2i delta = sf::Vector2i(lastPos_.x -pos.x,lastPos_.y-pos.y);


    if(state_ == DRAGING){
      for(uint id : selected_){
        spritelayer->shiftSprite(id,delta);
        
      }
    }



      lastPos_ = pos;
    }






  }


void SpriteSelection::onRelease() {
  std::shared_ptr<Layer> layer = map_->getCurrentLayer();
  if (layer->getType() == SPRITELAYER) {
    std::shared_ptr<SpriteLayer> spritelayer = static_pointer_cast<SpriteLayer>(layer);

    if(state_ == SELECTION){
      sf::FloatRect selectionRect;
      selectionRect.size.x = std::max(startSelectionPos_.x,lastPos_.x) - std::min(startSelectionPos_.x,lastPos_.x);
      selectionRect.size.y = std::max(startSelectionPos_.y,lastPos_.y) - std::min(startSelectionPos_.y,lastPos_.y);
      selectionRect.position.x =(startSelectionPos_.x + lastPos_.x)/2 ;
      selectionRect.position.y = (startSelectionPos_.y+lastPos_.y)/2;
      const std::vector<SpriteObject> &sprites = spritelayer->getSprites();
      for(const auto& sprite : sprites){
        if(selectionRect.findIntersection(sprite.sprite.getGlobalBounds())){
          if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)){
            
            
          }

        }

      }

      


    }








  isDrawing_ = false;
  }
}
