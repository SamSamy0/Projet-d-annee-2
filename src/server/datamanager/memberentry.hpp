#pragma once
#include <cstdint>
#include <string>


struct MemberEntry {
    long long userId;
    int8_t role;
    std::string pseudo;
};