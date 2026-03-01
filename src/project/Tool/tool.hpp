#pragma once
#include <SFML/Graphics.hpp>

class ClientNetworkManager;
class Map;
class Layer;

enum toolType{PIXELBRUSH,
PIXELSHIFT,
SPRITEBRUSH,};

enum Shape { SQUARE, DIAMOND, CIRCLE };




//-------------------------TOOL------------------------------------
class Tool {
protected:
  std::shared_ptr<Map> map_;
  toolType name_;
  bool isDrawing_ = false;
  sf::Vector2i lastPos_;

public:
  Tool(std::shared_ptr<Map> map);
  // virtual void getMessage(const ClientNetworkManager &netw
  // virtual void sendMessage(const ClientNetworkManager &network) const;
  //Getter
  std::shared_ptr<Map> getMap();
  unsigned int getScale();


  //Action
  virtual void onPress(sf::Vector2i pos) = 0;
  virtual void onDrag(sf::Vector2i pos) = 0;
  virtual void onRelease() = 0;

  virtual ~Tool() = default;
};

//-----------------------------PIXELBRUSH---------------------------
//-----------------------------------------------------------------------




