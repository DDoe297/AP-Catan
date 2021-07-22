#include "Utility.hpp"
int randomNumber(int start, int end) {
    return rand() % (end - start + 1) + start;
}
