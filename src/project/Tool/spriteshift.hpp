#pragma once
#include "shift.hpp"
#include "../../client/clientnetwork.hpp"

class SpriteShift : public Shift {

public:
  SpriteShift(std::shared_ptr<Map> map, ClientNetworkManager& manager);
};
