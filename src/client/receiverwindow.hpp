#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "../Window/Window.hpp"


class Window;             // Forward declaration

class ReceiverInWindow{
    Window* window_;
    
public:
    ReceiverInWindow(Window& w);
    void switchConnectState(uint8_t connect);
    void addProjectToList(ProjectData projet);
    void updateProjectNameInList(long long id, const std::string& name);
};
