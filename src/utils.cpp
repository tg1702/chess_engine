#include "utils.h"

#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "types.h"

namespace utils {
std::vector<std::string> split_string(std::string str) {
  std::stringstream ss(str);
  std::istream_iterator<std::string> begin(ss), end;
  return std::vector<std::string>(begin, end);
}

};  // namespace utils