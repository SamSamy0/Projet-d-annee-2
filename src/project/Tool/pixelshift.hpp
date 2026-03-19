#pragma once
#include "shift.hpp"
#include "../../client/clientnetwork.hpp"
class PixelShift : public Shift {

public:
  PixelShift(std::shared_ptr<Map> map, ClientNetworkManager &manager);
};
