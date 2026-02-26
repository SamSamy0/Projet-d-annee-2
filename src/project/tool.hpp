#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

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

//-------------- BRUSH------------------------------------------------
class Brush :public Tool {
protected:
  sf::Vector2u size_m_ = sf::Vector2u(1, 1);
  int distance_ = 0.0f;
  int spacing_;
public:
  Brush(std::shared_ptr<Map>);
  void setSize(unsigned int x, unsigned int y);
  sf::Vector2u getSize();
  void onPress(sf::Vector2i pos);
  void onDrag(sf::Vector2i pos);
  void onRelease();
  virtual void paint(sf::Vector2i pos) = 0;
  virtual ~Brush() = default;
};
//-----------------------------PIXELBRUSH---------------------------
class PixelBrush :public Brush {
  sf::Color color_ = sf::Color::Black;
  Shape shape_ = SQUARE;
  bool is_erraser_ = false;

public:
  PixelBrush(std::shared_ptr<Map> map);
  void setColor(sf::Color c);
  void setShape(Shape s);
  void paint(sf::Vector2i pos);
  void setErraser(bool val);
};
//-----------------------------------------------------------------------




class PixelShift : public Tool {



public:
  PixelShift(std::shared_ptr<Map> map);
  void onPress(sf::Vector2i pos)override;
  void onDrag(sf::Vector2i pos)override;
  void onRelease()override;
  void shiftOn(sf::Vector2i pos_1) ;
};
