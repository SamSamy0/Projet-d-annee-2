#include "ProjectData.hpp"
#include <iostream>

void ProjectData::print() {
  std::cout << "########### " << projectName << " ##########" << std::endl;
  std::cout << "role :" << static_cast<int>(role) << std::endl;
  std::cout << "id :" << static_cast<int>(projectId) << std::endl;
  std::cout << "#############################################" << std::endl;
}

ProjectData::ProjectData(uint projectId, int8_t role, std::string projectName)
    : projectId{projectId}, role{role}, projectName{projectName} {};
void ProjectData::setName(std::string newName) { projectName = newName; }
