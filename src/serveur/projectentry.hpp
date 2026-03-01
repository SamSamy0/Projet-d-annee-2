#pragma once
#include <cstdint>
#include <string>

struct ProjectEntry {
    long long projectId;
    int8_t role;
    std::string name;
};