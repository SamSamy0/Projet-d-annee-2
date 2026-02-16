#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class ClientNetworkManager;
class Map;
class Layer;

enum typeOutil{PIXELBRUSH,
PIXELSHIFT,
SPRITEBRUSH,};

enum Shape { SQUARE, DIAMOND, CIRCLE };




//-------------------------TOOL------------------------------------
class Tool {
protected:
  std::shared_ptr<Map> map_;
  typeOutil name_;

public:
  Tool(std::shared_ptr<Map> map);
  //Getter
  std::shared_ptr<Map> getMap();
  unsigned int getScale();

  //Setter
  virtual void setColor(sf::Color c){}
  virtual void setShape(Shape s){}
  virtual void setSize(unsigned int x, unsigned int y){}
  virtual void setErraser(){}

  //Action
  virtual void shiftOn(sf::Vector2u pos_1, sf::Vector2u pos_2){}
  virtual void drawOn(sf::Vector2i pos) {};
  // virtual void getMessage(const ClientNetworkManager &netw
  // virtual void sendMessage(const ClientNetworkManager &network) const;
  virtual ~Tool() = default;
};

//-------------- BRUSH------------------------------------------------
class Brush :public Tool {
protected:
  sf::Vector2u size_m_ = sf::Vector2u(1, 1);
public:
  Brush(std::shared_ptr<Map>);
  void setSize(unsigned int x, unsigned int y);
  virtual void drawOn(sf::Vector2i pos) = 0;
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
  void drawOn(sf::Vector2i pos) override;
  void setErraser() override ;
};
//-----------------------------------------------------------------------




class PixelShift : public Tool {



public:
  PixelShift(std::shared_ptr<Map> map);
  void shiftOn(sf::Vector2u pos_1, sf::Vector2u pos_2) override;
};
