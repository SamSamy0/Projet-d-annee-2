#pragma once
#include <stdint.h>
#include <string>
#include <vector>

class User {
  std::string pseudo_;
  uint id_;

public:
  User() = default;
  User(const std::string &pseudo, const uint id) : pseudo_(pseudo), id_{id} {}
  std::string getUser() const { return pseudo_; }
  uint getId() const { return id_; }
  void setId(uint newId) { id_ = newId; }
};
