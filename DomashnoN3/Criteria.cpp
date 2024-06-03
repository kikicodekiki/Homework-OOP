#include "Criteria.h"
Criteria::Criteria(int32_t *newArgs, int16_t N) {
    this -> N = N;

    for (size_t i = 0; i < MAX_SIZE; i++) {
        arguments[i] = newArgs[i];
    }
}
