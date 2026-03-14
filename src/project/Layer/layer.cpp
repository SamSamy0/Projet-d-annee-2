#include "layer.hpp"

Layer::Layer(std::string name, sf::Vector2u size) : name_{name}, size_{size} {}


std::string Layer::getName() const { return name_; }

bool Layer::getMasked() const { return masked_; }
void Layer::setMasked(bool masked) { masked_ = masked; }

LayerType Layer::getType() { return type_; }
