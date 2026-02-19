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
  //Getter
  std::shared_ptr<Map> getMap();
  unsigned int getScale();
  sf::Vector2u getSize(){}

  //Setter
  virtual void setColor(sf::Color c){}
  virtual void setShape(Shape s){}
  virtual void setSize(unsigned int x, unsigned int y){}
  virtual void setErraser(bool val){}

  //Action
  virtual void shiftOn(sf::Vector2i pos_1){}
  virtual void onPress(sf::Vector2i pos){}
  virtual void onDrag(sf::Vector2i pos){}
  virtual void onRelease(){}

  virtual void drawOn(sf::Vector2i pos) {}
  // virtual void getMessage(const ClientNetworkManager &netw
  // virtual void sendMessage(const ClientNetworkManager &network) const;
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
  void setColor(sf::Color c) override;
  void setShape(Shape s)override;
  void paint(sf::Vector2i pos) override;
  void setErraser(bool val) override ;
};
//-----------------------------------------------------------------------




class PixelShift : public Tool {



public:
  PixelShift(std::shared_ptr<Map> map);
  void onPress(sf::Vector2i pos)override;
  void onDrag(sf::Vector2i pos)override;
  void onRelease()override;
  void shiftOn(sf::Vector2i pos_1) override;
};
