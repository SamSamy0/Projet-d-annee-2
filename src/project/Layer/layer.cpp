#include "layer.hpp"

Layer::Layer(uint id,std::string name, sf::Vector2u size) : id_{id}, name_{name}, size_{size} {}


std::string Layer::getName() const { return name_; }
void Layer::setName(const std::string& name) { name_ = name; }

uint Layer::getId()const{return id_;}

bool Layer::isMasked() const { return masked_; }
void Layer::setMasked(bool masked) { masked_ = masked; }

LayerType Layer::getType() { return type_; }
