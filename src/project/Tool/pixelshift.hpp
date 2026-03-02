#pragma once
#include "tool.hpp"
class PixelShift : public Tool {



public:
  PixelShift(std::shared_ptr<Map> map);
  void onPress(sf::Vector2i pos)override;
  void onDrag(sf::Vector2i pos)override;
  void onRelease()override;
  void shiftOn(sf::Vector2i pos_1) ;
};
