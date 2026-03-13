#pragma once
#include <string>

class User {
    std::string pseudo_;
    public :
        User() = default;
        User(const std::string& pseudo) : pseudo_(pseudo) {}
        std::string getUser() const {return pseudo_;}
};