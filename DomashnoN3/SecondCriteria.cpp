#include "SecondCriteria.h"
int SecondCriteria::pointIndex(int32_t point) const {
    for (size_t i = 0; i < N; i++) {
        if (point == arguments[i]) {
            return i;
        }
    }
    return -1;
}

Pair<bool,int32_t> SecondCriteria:: operator()(int32_t point) const {
    static int invalid = 0;

    Pair <bool, int32_t> result;
    int index = pointIndex(point);
    if (index == -1) {
        result.setFirst(true);
        result.setSecond(point);
        return result;
    }

    result.setFirst(false);
    result.setSecond(invalid);
    return result;
}

SecondCriteria::SecondCriteria(int32_t *args, int16_t N) : Criteria(args, N)  {}
