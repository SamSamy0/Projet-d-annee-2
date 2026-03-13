#include "layer.hpp"
#include <memory>

Layer::Layer(std::string name, sf::Vector2u size) : name_{name}, size_{size} {}

typeCouche Layer::getType(){return type_;}

std::string Layer::getName() const { return name_; }

bool Layer::getMasked() const { return Masked_; }
void Layer::setMasked(bool masked) { Masked_ = masked; }

