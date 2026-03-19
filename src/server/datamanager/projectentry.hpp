#pragma once
#include <cstdint>
#include <string>

struct ProjectEntry {
    uint projectId;
    int8_t role;
    std::string name;
};