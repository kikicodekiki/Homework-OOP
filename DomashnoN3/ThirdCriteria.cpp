#include "ThirdCriteria.h"

ThirdCriteria :: ThirdCriteria(int32_t* args, int16_t N) : Criteria(args, N) {}

int ThirdCriteria::pointIndex(int32_t point) const {
    static int invalidArgument = -1;

    for (int i = 0; i < N; i++) {
        if (arguments[i] == point) {
            return i;
        }
    }
    return invalidArgument;
}

Pair<bool,int> ThirdCriteria::operator()(int32_t point) const {
    Pair<bool,int> result ;

    static int validArgument = 1;
    static int invalidArgument = 0;

    int index = pointIndex(point);
    if(index == -1) {
        result.setFirst(true);
        result.setSecond(invalidArgument);
        return result;
    }

    result.setFirst(true);
    result.setSecond(validArgument);
    return result;
}
