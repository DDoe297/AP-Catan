#include "Utility.hpp"
int randomNumber(int start, int end) {
  srand(time(NULL));
  return std::rand() % (end - start + 1) + start;
}
