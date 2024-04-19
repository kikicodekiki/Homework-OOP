#pragma once
#include <iostream>

class ModifiableIntegersFunction {
    int16_t (*ogFunc) (int16_t); // func pointer

    int16_t results [UINT16_MAX]; //to hold te modified results; using uint16_t's range

    bool isModified [UINT16_MAX]; //flag to check modifications

    bool isExcluded [UINT16_MAX];


public:
    ModifiableIntegersFunction(int16_t(*myFunc)(int16_t));
    ~ModifiableIntegersFunction();

    void setResultForSpecificInput (int16_t input, int16_t wantedOutput);
    void excludePoint (int16_t input);

    int16_t operator() (int16_t x) const; //func call operator





};
