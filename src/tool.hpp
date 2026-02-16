#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class ClientNetworkManager;
class Map;
class Layer;

enum typeOutilsPixel{PIXELBRUSH,
PIXELSHIFT,
SPRITEBRUSH,};
enum Shape { SQUARE, DIAMOND, CIRCLE };

//-------------------------TOOL------------------------------------
class Tool {
protected:
  std::shared_ptr<Map> map_;
  bool is_selected_ = false;
  typeOutilsPixel name_;

public:
  Tool(std::shared_ptr<Map> map);
  std::shared_ptr<Map> getMap();
  unsigned int getScale();
  // virtual void getMessage(const ClientNetworkManager &network) const;
  // virtual void sendMessage(const ClientNetworkManager &network) const;
  virtual ~Tool() = default;
};

//-------------- BRUSH------------------------------------------------
class Brush {
protected:
  sf::Vector2u size_m_ = sf::Vector2u(0, 0); // WARNING: (la valeur par défaut peut changer)
public:
  Brush() = default;
  void setSize(unsigned int x, unsigned int y);
  virtual void drawOn(sf::Vector2u pos) = 0;
  virtual ~Brush() = default;
};
//-----------------------------PIXELBRUSH---------------------------
class PixelBrush : public Tool, public Brush {
  sf::Color color_;
  Shape shape_;
  bool is_erraser_ = false;

public:
  PixelBrush(std::shared_ptr<Map> map);
  void setColor(sf::Color c);
  void setShape(Shape s);
  void drawOn(sf::Vector2u pos);
  void setErraser();
};
//-----------------------------------------------------------------------




class PixelShift : public Tool {



public:
  PixelShift(std::shared_ptr<Map> map);
  void shiftOn(sf::Vector2u pos_1, sf::Vector2u pos_2);
};
class SpriteTool : public Tool {};
