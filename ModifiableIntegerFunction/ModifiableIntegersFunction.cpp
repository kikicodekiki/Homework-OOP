#pragma once
#include "ModifiableIntegersFunction.h"
#include <iostream>
#include <cstdint>  // For int16_t, uint16_t, UINT16_MAX
#include <stdexcept>

ModifiableIntegersFunction ::ModifiableIntegersFunction(int16_t (*myFunc)(int16_t)) : ogFunc(myFunc) {

    results = new int16_t* [MAX_SIZE] {nullptr};

    for(int i = 0; i < MAX_SIZE; i++) {
        isModified[i] = false;
        isExcluded[i] = false;
    }

}

void ModifiableIntegersFunction::free() {

    for (int i = 0; i < MAX_SIZE; i++) {

        delete results[i];
        results[i] = nullptr;
        isModified[i] = false;
        isExcluded[i] = false;

    }

    delete[] results;
    results = nullptr;
}



void ModifiableIntegersFunction::copyFrom (const ModifiableIntegersFunction& other) {

    ogFunc = other.ogFunc;

    results = new int16_t * [MAX_SIZE] {nullptr};

    for (int i = 0;  i < MAX_SIZE; i++) {

        if (other.isExcluded[i]) {
            results[i] = nullptr; //explicitly setting them to minimise logical errors
            isExcluded[i] = true;
            isModified[i] = false;
        } else if (other.results[i] != nullptr) {

            results[i] = new int16_t(*(other.results[i])); //to reuse the same 'box' as our lecturer might say
            isModified[i] = other.isModified[i];
            isExcluded[i] = false;

        } else {
            //means that the point is active but just not initialised in order to save time
            isModified[i] = false;
            isExcluded[i] = false;
        }

    }


}

ModifiableIntegersFunction::ModifiableIntegersFunction (const ModifiableIntegersFunction& other) {
    copyFrom(other);
}

void ModifiableIntegersFunction::setResultForSpecificInput (int16_t input, int16_t wantedOutput) {

    uint16_t newIndex = (uint16_t) (input + INT16_MAX + 1); // so that I can find the correct index (-32768, +32768)

    if (results[newIndex] == nullptr) {
        results[newIndex] = new int16_t(wantedOutput);
    } else {
        *results[newIndex] = wantedOutput;
    }
    isModified[newIndex] = true;
    isExcluded[newIndex] = false;

}

void ModifiableIntegersFunction::excludePoint(int16_t input) {
    uint16_t index = (uint16_t) (input + INT16_MAX + 1);

    isExcluded[index] = true;

    if (results[index] != nullptr) {
        delete results[index];
        results[index] = nullptr;
    }

    isModified[index] = false;

}

bool ModifiableIntegersFunction::isPointExcluded (int16_t input) const {
    uint16_t index = (uint16_t) (input + INT16_MAX + 1);
    return isExcluded[index];
}

ModifiableIntegersFunction::~ModifiableIntegersFunction() noexcept {
    free();
}

ModifiableIntegersFunction& ModifiableIntegersFunction:: operator= (const ModifiableIntegersFunction& other) {
    if (this != &other) {
        free();
        copyFrom(other);
    }

    return *this;
}

int16_t ModifiableIntegersFunction::operator()(int16_t x) const {

    uint16_t index = (uint16_t)(x + INT16_MAX + 1);

    if(isExcluded[index]) {
        throw std::runtime_error ("point used in ()operator is excluded!");
    }

    return isModified[index] ? (*results[index]) : ogFunc(x);

}

bool ModifiableIntegersFunction::isPointNull(int16_t input) const {
    uint16_t index = (uint16_t) (input + INT16_MAX + 1);
    return results[index] == nullptr;
}

ModifiableIntegersFunction& ModifiableIntegersFunction::operator+=(const ModifiableIntegersFunction &rhs) {
    for (int i = 0; i < MAX_SIZE; i++) {

        int16_t input = (int16_t)(i - INT16_MAX - 1); // Correctly adjust index to signed int16_t range

        if (isPointExcluded(input) || rhs.isPointExcluded(input)) {
            if (!isPointExcluded(input) || !rhs.isPointExcluded(input)) {
                // adding a value to an excluded point is undefined -> continue
                continue;
            } //both excluded -> also continue
            continue;
        }

        int16_t lhsValue = (!isPointNull(input)) ? *results[i] : ogFunc( input );
        int16_t rhsValue = (!rhs.isPointNull(input)) ? *rhs.results[i] : rhs.ogFunc( input );

        if ( isPointNull(input) ) {
            results[i] = new int16_t(lhsValue);
        }

        *results[i] += rhsValue;
        isModified[i] = true;
        isExcluded[i] = false;
    }
    return *this;
}

ModifiableIntegersFunction operator+ (const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs) {

    ModifiableIntegersFunction lhsCopy (lhs); //this is a large object so maybe not the most efficient thing to do lol
    lhsCopy += rhs;

    return lhsCopy;
}

ModifiableIntegersFunction& ModifiableIntegersFunction::operator-= (const ModifiableIntegersFunction& rhs) {
    for (int i = 0; i < MAX_SIZE; i++) {
        int16_t leftValue = (results[i] != nullptr) ? *results[i] : ogFunc(i - INT16_MAX);
        int16_t rightValue = (rhs.results[i] != nullptr) ? *rhs.results[i] : rhs.ogFunc(i - INT16_MAX);

        if (results[i] == nullptr && rhs.results[i] == nullptr && !isModified[i] && !rhs.isModified[i]) {
            continue;  // Optionally, skip to avoid unnecessary modifications
        }

        if (results[i] == nullptr) {
            results[i] = new int16_t(leftValue);
        }
        *results[i] -= rightValue;
        isModified[i] = true;
    }
    return *this;
}


ModifiableIntegersFunction operator- (const ModifiableIntegersFunction& lhs, const ModifiableIntegersFunction& rhs) {

    ModifiableIntegersFunction lhsCopy(lhs);
    lhsCopy -= rhs;

    return lhsCopy;
}

ModifiableIntegersFunction operator*(const ModifiableIntegersFunction& f, const ModifiableIntegersFunction& g) {
    //will use the * operator for composition
    //ModifiableIntegersFunction composition(f); // creating a copy





}
