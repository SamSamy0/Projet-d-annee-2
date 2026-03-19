#include "pixelshift.hpp"
#include "../Layer/pixellayer.hpp"
#include "../map.hpp"

PixelShift::PixelShift(std::shared_ptr<Map> map, ClientNetworkManager &manager) : Shift(map, manager) {
  type_ = PIXELSHIFT;
}
