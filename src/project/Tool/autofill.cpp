#include "autofill.hpp"
#include "../map.hpp"





AutoFill::AutoFill(std::shared_ptr<Map> map, ClientNetworkManager &manager) : Tool(map,manager){ type_ = AUTOFILL;}





void AutoFill::onPress(sf::Vector2i pos){
  startPos_ = pos;
}
void AutoFill::onDrag(sf::Vector2i pos){}
void AutoFill::onRelease(){}
