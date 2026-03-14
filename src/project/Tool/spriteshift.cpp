#include "spriteshift.hpp"

SpriteShift::SpriteShift(std::shared_ptr<Map> map) : Shift(map) {
  type_ = SPRITESHIFT;
}
