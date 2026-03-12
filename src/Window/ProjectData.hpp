#include <SFML/Graphics.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <cstdint>

struct ProjectData {
  long long projectId;
  int8_t role;
  std::string projectName;

  void print();
};
