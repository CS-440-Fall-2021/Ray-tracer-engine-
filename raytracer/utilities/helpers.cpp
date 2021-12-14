#include "helpers.hpp"
#include <algorithm>

float clip(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}

void swap(float &a, float &b) {
  float temp = a;
  a = b;
  b = temp;
}