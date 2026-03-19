#pragma once
#include <cstdint>
#include <string>


struct MemberEntry {
    uint userId;
    int8_t role;
    std::string pseudo;
};