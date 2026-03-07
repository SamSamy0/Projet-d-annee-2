#include "pixelshift.hpp"
#include "../Layer/pixellayer.hpp"
#include "../map.hpp"



PixelShift::PixelShift(std::shared_ptr<Map> map) : Tool(map) {
  type_ = PIXELSHIFT;
}


void PixelShift::onPress(sf::Vector2i pos) {
  isDrawing_ = true;
  lastPos_ = pos;
}
void PixelShift::onDrag(sf::Vector2i pos) {
  if (!isDrawing_)
    return;

  sf::Vector2i delta = pos - lastPos_;

  if (delta.x != 0 || delta.y != 0) {
    std::shared_ptr<Layer> layer = map_->getCurrentLayer();
    layer->shift(delta);
  }
  lastPos_ = pos;
}
void PixelShift::onRelease() {
  isDrawing_ = false;
  lastPos_ = sf::Vector2i(0,0);
}
