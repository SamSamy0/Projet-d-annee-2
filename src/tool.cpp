#include "tool.hpp"




void Brush::setSize(float s){ this->size_m_ = s;}


std::shared_ptr<Project> Tool::getProject(){return this->project;}
float Tool::getScale(){
  return project->getScale();
}


void PixelBrush::setColor(sf::Color c) {color_ = c;} //NOTE: PEUT ETRE FAIRE UNE FONCTION PAR R G B A
void PixelBrush::setShape(Shape s){shape_ = s;}
void PixelBrush::setErraser(){is_erraser_ = !is_erraser_;}

void PixelBrush::drawOn(Layer& couche, sf::Vector2f pos){
  float size = size_m_*getScale();
  PixelLayer* pixellayer = dynamic_cast<PixelLayer*>(&couche);

  switch(shape_){
    case SQUARE:{
      sf::RectangleShape square(sf::Vector2f(size,size));
      float offset = size /2.0f;
      square.setOrigin(sf::Vector2f(offset,offset));
      square.setPosition(pos);
      is_erraser_ ? square.setFillColor(sf::Color::Transparent) : square.setFillColor(color_);
      is_erraser_ ? pixellayer->errase(square) : pixellayer->draw(square);
      break;}
    case CIRCLE:{
      float radius = size/2.0f;
      sf::CircleShape circle(radius);
      circle.setOrigin(sf::Vector2f(radius,radius));
      circle.setPosition(pos);
      is_erraser_ ? circle.setFillColor(sf::Color::Transparent) : circle.setFillColor(color_);
      is_erraser_ ? pixellayer->errase(circle) : pixellayer->draw(circle);
      break;}
    case DIAMOND:{
      //TODO: faire pour le losange
      break;}
  }

}

