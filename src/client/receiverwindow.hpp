#pragma once

#include <vector>
#include <string>
#include <cstdint>
// #include "../Window/Window.hpp"
#include "../Window/Application.hpp"


// class Window;             // Forward declaration

class ReceiverInWindow{
    Application* app_;
    
public:
    ReceiverInWindow(Application& w);
    void switchConnectState(uint8_t connect);
    void addProjectToList(ProjectData projet);
    void updateProjectNameInList(uint id, const std::string& name);
    void updateCreatedProjectId(uint ProjectId);
    void setState();
    void setProject(uint scale, uint height, uint width , std::string name, uint id);
};
