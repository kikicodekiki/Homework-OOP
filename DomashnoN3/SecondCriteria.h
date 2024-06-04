#pragma once
#include <iostream>
#include "Pair.h"
#include "Criteria.h"

class SecondCriteria: public Criteria { // T==1
public:
    SecondCriteria(int32_t* args, int16_t N);
    Pair<bool,int32_t> operator()(int32_t point) const override;

};
