#pragma once
#include "Pair.h"
#include <iostream>

class Criteria {
public:
    static constexpr size_t MAX_SIZE = 32;

    Criteria (int32_t* newArgs, int16_t N);
    virtual Pair<bool,int32_t> operator()(int32_t point) const = 0;
    virtual ~Criteria() = default;
protected:
    int32_t arguments[MAX_SIZE];
    int16_t N;
    virtual int pointIndex (int32_t point) const = 0;
};

constexpr size_t Criteria::MAX_SIZE;


