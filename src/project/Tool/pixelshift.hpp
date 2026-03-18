#pragma once
#include "shift.hpp"
class PixelShift : public Shift {

public:
  PixelShift(std::shared_ptr<Map> map, ClientNetworkManager &manager);
};
