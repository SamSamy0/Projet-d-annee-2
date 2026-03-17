#include "spriteshift.hpp"

SpriteShift::SpriteShift(std::shared_ptr<Map> map, ClientNetworkManager& manager) : Shift(map, manager) {
  type_ = SPRITESHIFT;
}
