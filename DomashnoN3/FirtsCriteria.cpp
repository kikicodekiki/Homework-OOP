#pragma once
#include <iostream>
#include "Pair.h"
#include "Criteria.h"

class FirstCriteria : public Criteria { // T == 0
public:
    FirstCriteria (int32_t* newArgs, int32_t* newResults, int16_t N);
    Pair<bool,int32_t> operator()(int32_t point) const override; //defined everywhere

private:
    int32_t results[MAX_SIZE];
    int pointIndex (int32_t point) const override;

};

