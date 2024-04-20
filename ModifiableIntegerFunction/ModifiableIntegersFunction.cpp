#pragma once
#include "ModifiableIntegersFunction.h"
#include <iostream>
#include <cstdint>  // For int16_t, uint16_t, UINT16_MAX
#include <stdexcept>
#include <fstream>

const int ModifiableIntegersFunction:: MAX_SIZE ;

ModifiableIntegersFunction ::ModifiableIntegersFunction(int16_t (*myFunc)(int16_t)) : ogFunc(myFunc) {

    results = new int16_t* [MAX_SIZE] {nullptr};

    for(int i = 0; i < MAX_SIZE; i++) {
        isModified[i] = false;
        isExcluded[i] = false;
    }

}

void ModifiableIntegersFunction::free() noexcept {

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
        if (isPointExcluded(i) || rhs.isPointExcluded(i)) {
            continue;  // skip for excluded points
        }

        // ensure results is initialized for the current idx
        if (results[i] == nullptr && (ogFunc != nullptr || rhs.ogFunc != nullptr)) {
            int16_t thisVal = ogFunc ? ogFunc(i - INT16_MAX - 1) : 0;
            results[i] = new int16_t(thisVal);
        }

        // subtract values if the rhs result is not nullptr
        if (rhs.results[i] != nullptr) {
            *results[i] -= *rhs.results[i];
        } else if (rhs.ogFunc != nullptr) { // use original function if no specific result is set
            *results[i] -= rhs.ogFunc(i - INT16_MAX - 1);
        }

        // mark index as modified
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
    ModifiableIntegersFunction composition; // using the default constructor with the dummy function

    for (int i = 0; i < ModifiableIntegersFunction::MAX_SIZE; i++ ) {

        int16_t input = (int16_t)(i - INT16_MAX - 1);

        try {
            //apply f and then g
            int16_t resultOfg = g(input);
            int16_t finalResult = f(resultOfg);

            composition.setResultForSpecificInput(input, finalResult);

        } catch (const std::runtime_error& ) {
            //an error is thrown when a point is excluded so that means
            // that the composition should also exclude it
            composition.excludePoint(input);
        }

    }

    return composition;
}

bool operator< (const ModifiableIntegersFunction& f, const ModifiableIntegersFunction& g) {

    for (int i = 0 ; i < ModifiableIntegersFunction::MAX_SIZE; i++) {
        int16_t input = (int16_t)(i - INT16_MAX - 1);

        int16_t fResult, gResult;
        fResult = gResult = INT16_MIN; //default

        if (!f.isPointExcluded(input)) {
            try {
                fResult = f(input);
            } catch (const std::runtime_error&) {
                fResult = INT16_MIN; //if it's excluded it sets it to the min possible value
            }
        }

        if (!g.isPointExcluded(input)) {
            try {
                gResult = g(input);
            } catch (const std::runtime_error&) {
                gResult = INT16_MIN;
            }
        }

        if (fResult >= gResult) { //checking for the opposite
            return false;
        }

    }

    return true; //the comparison is valid
}

bool operator> (const ModifiableIntegersFunction& f, const ModifiableIntegersFunction& g) {
    for (int i = 0 ; i < ModifiableIntegersFunction::MAX_SIZE; i++) {
        int16_t input = (int16_t)(i - INT16_MAX - 1);

        int16_t fResult, gResult;
        fResult = gResult = INT16_MIN; //default

        if (!f.isPointExcluded(input)) {
            try {
                fResult = f(input);
            } catch (const std::runtime_error&) {
                fResult = INT16_MIN; //if it's excluded it sets it to the min possible value
            }
        }

        if (!g.isPointExcluded(input)) {
            try {
                gResult = g(input);
            } catch (const std::runtime_error&) {
                gResult = INT16_MIN;
            }
        }

        if (fResult <= gResult) { //checking for the opposite
            return false;
        }

    }

    return true; //the comparison is valid
}

bool operator== (const ModifiableIntegersFunction& f, const ModifiableIntegersFunction& g) {
    for (int i = 0 ; i < ModifiableIntegersFunction::MAX_SIZE; i++) {
        int16_t input = (int16_t)(i - INT16_MAX - 1);

        int16_t fResult, gResult;
        fResult = gResult = INT16_MIN; //default

        if (!f.isPointExcluded(input)) {
            try {
                fResult = f(input);
            } catch (const std::runtime_error&) {
                fResult = INT16_MIN; //if it's excluded it sets it to the min possible value
            }
        }

        if (!g.isPointExcluded(input)) {
            try {
                gResult = g(input);
            } catch (const std::runtime_error&) {
                gResult = INT16_MIN;
            }
        }

        if (fResult != gResult) { //checking for the opposite
            return false;
        }

    }

    return true;
}

bool operator!= (const ModifiableIntegersFunction& f, const ModifiableIntegersFunction& g) {
    return !(f == g);
}

bool operator<= (const ModifiableIntegersFunction& f, const ModifiableIntegersFunction& g) {
    return ( f < g ) || (f == g);
}

bool operator>= (const ModifiableIntegersFunction& f, const ModifiableIntegersFunction& g) {
    return (f > g) || (f == g);
}

bool operator|| (const ModifiableIntegersFunction& f, const ModifiableIntegersFunction& g) {

    double initialRatio = 0.0;
    bool ratioSet = false;  // flag to see if the initial ratio has been set

    for (int i = 0; i < ModifiableIntegersFunction::MAX_SIZE; i++) {
        int16_t input = (int16_t)(i - INT16_MAX - 1);

        if (f.isPointExcluded(input) || g.isPointExcluded(input)) {
            if (f.isPointExcluded(input) && g.isPointExcluded(input)) {
                continue;  // both excluded
            }
            return false;  // only one is excluded
        }

        int16_t fResult = f(input);
        int16_t gResult = g(input);


        //division by zero
        if (gResult == 0 && fResult ==0) {
            continue;
        }

        if(gResult == 0) {
            return false;
        }

        double currentRatio = (double)(fResult) / gResult;

        if (!ratioSet) {
            initialRatio = currentRatio; //set the initial ratio difference
            ratioSet = true;
        } else if (std::fabs(currentRatio - initialRatio) > 1e-9) {  // epsilon for two doubles comparison
            return false;
        } else if (std::fabs(currentRatio - initialRatio) < 1e-9) {
            return true;
        }
    }

    return true;

}


ModifiableIntegersFunction operator^ (const ModifiableIntegersFunction& f, unsigned k) {
    if (k == 0) {
        //return identity function
        return ModifiableIntegersFunction(dummyFunc);
    }

    ModifiableIntegersFunction result(f); //f^1

    for (unsigned i = 1; i < k; i++) {
        result = result * f;
    }

    return result;

}

bool ModifiableIntegersFunction::isInjection() const {
    bool hasImage[UINT16_MAX + 1] = {false};

    for (int i = 0; i < MAX_SIZE; ++i) {

        int16_t input = (int16_t)(i - INT16_MAX - 1);

        if (!isPointExcluded(input) && !isModified[i]) {
            int16_t result = ogFunc(input);

            if (hasImage[result + INT16_MAX + 1]) {
                return false; // already mapped
            }

            hasImage[result + INT16_MAX + 1] = true;
        }
    }
    return true;
}

bool ModifiableIntegersFunction::isSurjection() const {

    bool hasPreimage[UINT16_MAX + 1] = {false};

    for (int i = 0; i < MAX_SIZE; ++i) {

        int16_t input = (int16_t)(i - INT16_MAX - 1);
        if (!isPointExcluded(input)) {
            int16_t result = operator()(input);

            hasPreimage[result + INT16_MAX + 1] = true;
        }
    }
    for (int i = 0; i < UINT16_MAX + 1; ++i) {
        if (!hasPreimage[i]) {
            return false; // output value that does not have any preimage
        }
    }
    return true;
}

bool ModifiableIntegersFunction::isBijection() const {
    return isInjection() && isSurjection();
}

ModifiableIntegersFunction ModifiableIntegersFunction::inverse() const {
    if (!isBijection()) { //only invertible when a bijection
        throw std::runtime_error("Function not invertible");
    }

    ModifiableIntegersFunction invFunc(this->ogFunc);  //reusing the original func pointer

    for (int i = 0; i < MAX_SIZE; ++i) {
        int16_t input = (int16_t)(i - INT16_MAX - 1);

        if (!isPointExcluded(input)) {
            int16_t output = (isModified[i] && results[i]) ? *results[i] : ogFunc(input);
            uint16_t newIndex = output + INT16_MAX + 1;

            if (invFunc.results[newIndex] != nullptr) {
                throw std::runtime_error("Attempting to map one output to multiple inputs");
            }

            invFunc.results[newIndex] = new int16_t(input);
            invFunc.isModified[newIndex] = true;
        } else {
            // exclude the corresponding output in the inverse
            int16_t output = (isModified[i] && results[i]) ? *results[i] : ogFunc(input);
            uint16_t newIndex = output + INT16_MAX + 1;
            invFunc.excludePoint(newIndex);
        }
    }

    return invFunc;
}

void ModifiableIntegersFunction::serialize (const char* fileName) const {
    std::ofstream out (fileName, std::ios::binary);

    if (!out.is_open()) {
        std::cerr << fileName << "unable to open";
        return;
    }

    for (int i = 0; i < MAX_SIZE; i++) {
        out.write ((const char*)&isModified[i], sizeof (isModified[i]));
        out.write ((const char*)&isExcluded[i], sizeof (isExcluded[i]));

        //write result if modified and not null
        if(isModified[i] && results[i] != nullptr) {
            int16_t value = *results[i];
            out.write ((const char*)& value, sizeof (int16_t));
        }
    }

    out.close();

}

void ModifiableIntegersFunction::deserialize(const char *fileName) {
    std::ifstream in(fileName, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << fileName << "unable to open";
        return;
    }

    //i am writing directly back into the same object of the class
    //because I have no idea how to write a function pointer and then read it lol
    //enjoy :)

    this->free(); // clear current data
    results = new int16_t*[MAX_SIZE] {nullptr};

    for (int i = 0; i < MAX_SIZE; ++i) {
        // read modification and exclusion flags
        in.read((char*)(&isModified[i]), sizeof(isModified[i]));
        in.read((char*)(&isExcluded[i]), sizeof(isExcluded[i]));

        if (isModified[i] && !isExcluded[i]) {
            int16_t value;
            in.read((char*)(&value), sizeof(value));
            results[i] = new int16_t(value);
        } else {
            results[i] = nullptr;
        }
    }

    in.close();
}

void ModifiableIntegersFunction::print(int x1, int x2, int y1, int y2) const {
    if (x2 - x1 != 19 || y2 - y1 != 19) {
        std::cerr << "Invalid range";
        return;
    }

    // 20x20 grid initialized with spaces
    char plot[20][20];
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            plot[i][j] = ' ';
        }
    }

    // plotting the function within the specified ranges
    for (int x = x1; x <= x2; ++x) {
        int16_t result = 0;
        try {
            result = this->operator()(x);
            if (result < y1 || result > y2) {
                continue; // skip values outside the y-range
            }

            // calculate the plot indices
            int xIndex = x - x1;
            int yIndex = y2 - result;

            if (xIndex >= 0 && xIndex < 20 && yIndex >= 0 && yIndex < 20) {
                plot[yIndex][xIndex] = '*';
            }
        } catch (const std::runtime_error& e) {
            // excluded or other error cases - no plot
            continue;
        }
    }

    // ptint out the matrix
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 20; ++j) {
            std::cout << plot[i][j];
        }
        std::cout << std::endl;
    }
}


