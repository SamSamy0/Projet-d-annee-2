#include "Identifier.hpp"
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Text.hpp>
#include <TGUI/Widgets/TextArea.hpp>

std::string Identifier::getrole() { return this->role; }
std::string Identifier::getusername() { return this->username; }
void Identifier::setRole(std::string newRole) { this->role = newRole; }
