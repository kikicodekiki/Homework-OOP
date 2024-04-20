#pragma once
#include <iostream>

int16_t dummyFunc (int16_t x) {
    return x;
}

int16_t (*myDummyFunc) (int16_t x) = dummyFunc;

class ModifiableIntegersFunction {
private:
    int16_t (*ogFunc) (int16_t); // func pointer

    static const int MAX_SIZE = UINT16_MAX + 1 ;

    //this was the most memory & time
    // efficient approach I could come up with for this task
    int16_t** results; //can either hold the point or be nullptr; using uint16_t's range

    bool isModified [MAX_SIZE]; //flag to check modification so as to not initialise the whole results array
    bool isExcluded [MAX_SIZE]; //flag to check if it has been modified

    //for big 4
    void free();
    void copyFrom (const ModifiableIntegersFunction& other);

    bool isPointExcluded (int16_t input) const;
    bool isPointNull ( int16_t input ) const;

public:

    ModifiableIntegersFunction(int16_t(*myFunc)(int16_t));
    //default using dummy func ptr
    ModifiableIntegersFunction() : ModifiableIntegersFunction(myDummyFunc) {}

    ~ModifiableIntegersFunction() noexcept;

    ModifiableIntegersFunction (const ModifiableIntegersFunction& other);

    ModifiableIntegersFunction& operator= (const ModifiableIntegersFunction& other);

    void setResultForSpecificInput (int16_t input, int16_t wantedOutput);
    void excludePoint (int16_t input);

    int16_t operator() (int16_t x) const; //func call operator

    ModifiableIntegersFunction& operator+= (const ModifiableIntegersFunction& rhs);
    ModifiableIntegersFunction& operator-= (const ModifiableIntegersFunction& rhs);

    friend ModifiableIntegersFunction operator* (const ModifiableIntegersFunction& f, const ModifiableIntegersFunction& g) ;
    friend ModifiableIntegersFunction operator+ (const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);
    friend ModifiableIntegersFunction operator- (const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs);


};
