#include "ModifiableIntegersFunction.h"
#include <iostream>
#pragma once

ModifiableIntegersFunction::ModifiableIntegersFunction(int16_t (*func)(int16_t)) {
    ogFunc = func;

    for ( int i = 0; i < UINT16_MAX; i++ ) {
        results[i] = 0;
        isExcluded[i] =
                isModified[i] = false;
    }

}

void ModifiableIntegersFunction::setResultForSpecificInput (int16_t input, int16_t wantedOutput) {
    //converting by offsetting the array indexes
     uint16_t index = (uint16_t) (input + INT16_MAX); // +32768 - max int16

     results[index] = wantedOutput;
     isModified[index] = true;
}

void ModifiableIntegersFunction::excludePoint(int16_t input) {
    // convert int16_t input to a uint16_t index by offsetting
    uint16_t index = (uint16_t)(input + INT16_MAX); // +32768
    isExcluded[index] = true;
}

int16_t ModifiableIntegersFunction::operator()(int16_t x) const {
    uint16_t index = (uint16_t)(x + INT16_MAX);  // Adjust index for full int16_t range

    if (isExcluded[index]) {
        throw std::runtime_error("Function not defined for this input");  // Or return a special value
    }

    // Return modified result if available, otherwise compute using the original function
    return isModified[index] ? results[index] : ogFunc(x);
}
