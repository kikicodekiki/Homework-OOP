#include "ThirdCriteria.h"

ThirdCriteria :: ThirdCriteria(int32_t* args, int16_t N) : Criteria(args, N) {}

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
