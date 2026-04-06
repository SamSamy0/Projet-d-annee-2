#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <cstdint>

struct ProjectData {
  uint projectId;
  int8_t role;
  std::string projectName;
  ProjectData(uint projectId, int8_t role, std::string projectName);
  ProjectData() {};
  void setName(std::string newName);

  void print();
};
