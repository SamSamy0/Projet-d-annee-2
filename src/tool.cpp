#include "tool.hpp"
#include "project.hpp"
#include "layer.hpp"




std::shared_ptr<Project> Tool::getProject(){return this->project_;}
unsigned int Tool::getScale(){return project_->getScale();}
Tool::Tool(std::shared_ptr<Project> project,typeOutilsPixel name):project_{project},name_{name}{}


void Brush::setSize(unsigned int x, unsigned int y = 0){size_m_.x = x; size_m_.y = y;}



PixelBrush::PixelBrush(std::shared_ptr<Project> project,typeOutilsPixel name) : Brush(),Tool(project,name){}
void PixelBrush::setColor(sf::Color c) {color_ = c;} //NOTE: PEUT ETRE FAIRE UNE FONCTION PAR R G B A
void PixelBrush::setShape(Shape s){shape_ = s;}
void PixelBrush::setErraser(){is_erraser_ = !is_erraser_;}

void PixelBrush::drawOn(Layer& couche, sf::Vector2u pos){
  PixelLayer* pixellayer = dynamic_cast<PixelLayer*>(&couche);
  sf::Vector2f rounded_pos = sf::Vector2f(std::round(pos.x),std::round(pos.y));

  switch(shape_){
    case SQUARE:{
      float size = (size_m_.x)*getScale();
      sf::RectangleShape square(sf::Vector2f(size,size));
      float offset = size /2.0f;
      square.setOrigin(sf::Vector2f(offset,offset));
      square.setPosition(rounded_pos);
      is_erraser_ ? square.setFillColor(sf::Color::Transparent) : square.setFillColor(color_);
      is_erraser_ ? pixellayer->errase(square) : pixellayer->draw(square);
      break;}
    case CIRCLE:{
      float size = (size_m_.x)*getScale();
      float radius = size/2.0f;
      sf::CircleShape circle(radius);
      circle.setOrigin(sf::Vector2f(radius,radius));
      circle.setPosition(rounded_pos);
      is_erraser_ ? circle.setFillColor(sf::Color::Transparent) : circle.setFillColor(color_);
      is_erraser_ ? pixellayer->errase(circle) : pixellayer->draw(circle);
      break;}
    case DIAMOND:{
      float size_x = (size_m_.x)*getScale();
      float size_y = (size_m_.y)*getScale();
      sf::ConvexShape diamond(4);
      diamond.setPoint(0, sf::Vector2f(size_x/2.0f,0)); // top point
      diamond.setPoint(1, sf::Vector2f(size_x/2.0f,size_y)); // botom point
      diamond.setPoint(2, sf::Vector2f(size_x,size_y/2.0f)); // right point
      diamond.setPoint(3, sf::Vector2f(size_x/2.0f,0)); // left point
      diamond.setOrigin(sf::Vector2f(size_x,size_y));
      diamond.setPosition(rounded_pos);
      is_erraser_ ? diamond.setFillColor(sf::Color::Transparent) : diamond.setFillColor(color_);
      is_erraser_ ? pixellayer->errase(diamond) : pixellayer->draw(diamond);
      
      break;}
  }

}

