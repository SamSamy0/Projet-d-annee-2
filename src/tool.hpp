#pragma once 
#include <SFML/Graphics.hpp>
#include <memory>
#include "project.hpp"
#include "layer.hpp"


//NOTE: Quand ces classes seront codés il faudra inclure les hpp et supprimer les déclarations 
class ClientNetworkManager;

enum Shape{
  SQUARE,
  DIAMOND,
  CIRCLE
};


class Tool{
protected:

  std::shared_ptr<Project> project;
  bool is_selected_;
  std::string name_;

public:
  std::shared_ptr<Project> getProject();
  float getScale();
  virtual void getMessage(const ClientNetworkManager& network) const;
  virtual void sendMessage(const ClientNetworkManager& network) const;
};

class Brush{
protected:
  float size_m_ = 0; //WARNING: (la valeur par défaut peut changer)
public:
  void setSize(float s);
  virtual void drawOn(Layer& couche,sf::Vector2f pos) = 0;
  virtual ~Brush() = default;

};



class PixelTool: public Tool{};

class PixelBrush : public PixelTool, public Brush{
  sf::Color color_;
  Shape shape_;
  bool is_erraser_;


public:
  void setColor(sf::Color c);
  void setShape(Shape s);
  void drawOn(Layer& couche,sf::Vector2f pos);
  void setErraser();
};


class PixelShift : public PixelTool{};
class SpriteTool : public Tool{};

