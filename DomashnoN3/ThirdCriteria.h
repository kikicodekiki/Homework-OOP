#pragma once
#include <iostream>
#include "Pair.h"
#include "Criteria.h"


class ThirdCriteria : public Criteria { //T == 2
public:
    ThirdCriteria(int32_t* args, int16_t N);
    Pair<bool,int32_t> operator() (int32_t point) const override;
};
