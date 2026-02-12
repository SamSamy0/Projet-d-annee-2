#pragma once 
#include <SFML/Graphics.hpp>
#include <memory>


class ClientNetworkManager;
class Project;
class Layer;

enum class typeOutilsPixel;  
enum Shape{
  SQUARE,
  DIAMOND,
  CIRCLE
};



class Tool{
protected:

  std::shared_ptr<Project> project_;
  bool is_selected_ = false;
  typeOutilsPixel name_;

public:
  Tool(std::shared_ptr<Project> project,typeOutilsPixel name);
  std::shared_ptr<Project> getProject();
  unsigned int getScale();
  virtual void getMessage(const ClientNetworkManager& network) const;
  virtual void sendMessage(const ClientNetworkManager& network) const;
  virtual ~Tool() = default;
};




class Brush{
protected:
  sf::Vector2u size_m_ = sf::Vector2u(0,0); //WARNING: (la valeur par défaut peut changer)
public:
  Brush() = default;
  void setSize(unsigned int x, unsigned int y);
  virtual void drawOn(Layer& couche,sf::Vector2f pos) = 0;
  virtual ~Brush() = default;
};




class PixelBrush : public Tool, public Brush{
  sf::Color color_;
  Shape shape_;
  bool is_erraser_ = false;


public:
  PixelBrush(std::shared_ptr<Project> project,typeOutilsPixel name); 
  void setColor(sf::Color c);
  void setShape(Shape s);
  void drawOn(Layer& couche,sf::Vector2u pos);
  void setErraser();
};


class PixelShift : public Tool{};
class SpriteTool : public Tool{};

