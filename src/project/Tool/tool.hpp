#pragma once
#include "../../client/clientnetwork.hpp"
#include <SFML/Graphics.hpp>
#include "../../client/clientnetwork.hpp"

class ClientNetworkManager;
class Map;
class Layer;

enum ToolType {
  NONETOOL,
  PIXELBRUSH,
  PIXELSHIFT,
  SPRITEBRUSH,
  SPRITEERASER,
  SPRITESHIFT,
  SPRITESELECTION,
};

enum Shape { // WARNING: PEUT ETRE PAS AU BON ENDROIT DANS LE CODE
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
  ClientNetworkManager &manager_;

public:
  Tool(std::shared_ptr<Map> map, ClientNetworkManager &manager);
  // Getter
  std::shared_ptr<Map> getMap();
  unsigned int getScale() const;
  ToolType getType() const;

  // Action
  virtual void onPress(sf::Vector2i pos) = 0;
  virtual void onDrag(sf::Vector2i pos) = 0;
  virtual void onRelease() = 0;

  virtual ~Tool() = default;
};
