#pragma once 
#include <SFML/Graphics.hpp>
#include <memory>

class Project;
class ClientNetworkManager;
enum Shape{
  Square,
  Diamond,
  Circle
};


class Tool{
protected:

  std::shared_ptr<Project> project;
  bool isSelected;
  std::string name;

public:
  void getMessage(const ClientNetworkManager& network) const;
  void sendMessage(const ClientNetworkManager& network) const;
};




class PixelTool: public Tool{};

class PixelBrush : public PixelTool{
  sf::Color color;
  Shape shape;
  bool isEraser;


public:
  void setColor(sf::Color color);
  void setErraserMode(bool b);
  void setShape(Shape s);
};

class PixelShift : public PixelTool{};


class SpriteTool : public Tool{};
