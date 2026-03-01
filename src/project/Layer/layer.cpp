#include "layer.hpp"
#include <memory>

Layer::Layer(std::string name, sf::Vector2u size) : name_{name}, size_{size} {}

typeCouche Layer::getType(){return type_;}

