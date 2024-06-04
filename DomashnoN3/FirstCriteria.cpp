#include "FirstCriteria.h"
FirstCriteria ::FirstCriteria (int32_t* newArgs, int32_t* newResults, int16_t N) : Criteria(newArgs, N) {

    for (size_t i = 0; i < MAX_SIZE; i++) {
        results[i] = newResults[i];
    }
}

Pair<bool,int32_t> FirstCriteria::operator()(int32_t point) const {
    Pair<bool,int32_t> result;
    size_t index = getIndex(point);

    static int invalidIndex = -1;
    static int invalidNum = 0;


    if (index == invalidIndex) {
        result.setFirst(false);
        result.setSecond(invalidNum);

        return result;
    }

    result.setFirst(true);
    result.setSecond(results[index]);

    return result;
}
