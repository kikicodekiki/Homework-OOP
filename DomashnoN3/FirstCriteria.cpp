#include "FirstCriteria.h"
FirstCriteria ::FirstCriteria (int32_t* newArgs, int32_t* newResults, int16_t N) : Criteria(newArgs, N) {

    for (size_t i = 0; i < MAX_SIZE; i++) {
        results[i] = newResults[i];
    }
}

int FirstCriteria::pointIndex(int32_t point) const {
    for (size_t i = 0; i < N; i++) {
        if (point == arguments[i]) {
            return i;
        }
    }
    throw std::out_of_range ("such point does not exist");
}

Pair<bool,int32_t> FirstCriteria::operator()(int32_t point) const {
    Pair<bool,int32_t> result;
    size_t index = pointIndex(point);

    result.setFirst(true);
    result.setSecond(results[index]);

    return result;
}
