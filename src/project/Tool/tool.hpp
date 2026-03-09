#pragma once
#include <SFML/Graphics.hpp>

class ClientNetworkManager;
class Map;
class Layer;

enum ToolType{
  PIXELBRUSH,
  PIXELSHIFT,
  SPRITEBRUSH,
  SPRITEERASER,
  SPRITESHIFT,
  };


enum Shape { //WARNING: PEUT ETRE PAS AU BON ENDROIT DANS LE CODE 
  SQUARE, 
  DIAMOND, 
  CIRCLE
};




class Tool {
protected:
  std::shared_ptr<Map> map_;
  ToolType type_;
  bool isDrawing_ = false;
  sf::Vector2i lastPos_;

public:
  Tool(std::shared_ptr<Map> map);
  // virtual void getMessage(const ClientNetworkManager &netw
  // virtual void sendMessage(const ClientNetworkManager &network) const;
  //Getter
  std::shared_ptr<Map> getMap();
  unsigned int getScale()const;
  ToolType getType()const;


  //Action
  virtual void onPress(sf::Vector2i pos) = 0;
  virtual void onDrag(sf::Vector2i pos) = 0;
  virtual void onRelease() = 0;

  virtual ~Tool() = default;
};





