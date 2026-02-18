#pragma once
#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Text.hpp>
#include <TGUI/Widgets/TextArea.hpp>
#include <string>

class Identifier {
private:
  std::string username;
  std::string password;
  // Not sure
  // Maybe replace std::string by "Role" class that somehow stores the
  // permissions of the role and the hierarchy between roles
  std::string role;

public:
  Identifier(std::string username, std::string password)
      : username{username}, password{password} {}
  void setRole(std::string Role);
  std::string getrole();
  std::string getusername();
};
