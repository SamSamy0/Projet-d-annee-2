#include <iostream>
#include <stdlib.h>
#include <unordered_map>
int main() {
  std::unordered_map<std::string, std::string> a;
  a.insert({"bonjour", "hello"});
  a.insert({"okay", "okay"});
  a.insert({"Comment", "How"});
  for (auto i = a.begin(); i != a.end(); i++) {
    std::cout << i->first << std::endl;
  }
  return 0;
}
