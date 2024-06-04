#include "Criteria.h"
Criteria::Criteria(int32_t *newArgs, int16_t N) {
    this -> N = N;

    for (size_t i = 0; i < N; i++) {
        arguments[i] = newArgs[i];
    }
}


int Criteria::getIndex(int32_t point) const {
    static int invalidIndex = -1;

    for (size_t i = 0; i < N; i++) {
        if (point == arguments[i]) {
            return i;
        }
    }
    return invalidIndex;
}
