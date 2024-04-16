#include <iostream>
#include "MultiSet.h"
#pragma once

unsigned MultiSet::getIndex (unsigned num) const {
    return (num*k) / 8;
}

unsigned MultiSet::getBitOffset(unsigned int num) const {
    return (num * k) % 8;
}

MultiSet ::MultiSet() : n(0), k(0), bucketSize(0), buckets(nullptr)
{ }

MultiSet ::MultiSet(unsigned n, unsigned numberBits) : n(n), k (numberBits), bucketSize((n*k + 7) / 8 ) {
    buckets = new uint8_t [bucketSize] {0};
}

void MultiSet::free() {
    delete[] buckets;
    buckets = nullptr;
    n = 0;
    k = 0;
}

void MultiSet::copyFrom (const MultiSet& other) {

    n = other.n;
    k = other.k;
    bucketSize = other.bucketSize;

    buckets = new uint8_t [bucketSize];

    for (size_t i = 0; i < bucketSize; i++) {
        buckets[i] = other.buckets[i];
    }
}

MultiSet :: MultiSet(const MultiSet& other) {
    copyFrom(other);
}

MultiSet:: ~MultiSet() {
    free();
}

MultiSet& MultiSet :: operator= (const MultiSet& other) {
    if (this != &other) {
        free();
        copyFrom (other);
    }

    return *this;
}

bool MultiSet :: addNumber (unsigned num) {

    if (num > n) {
        return false;
    }

    unsigned idx = getIndex(num);
    unsigned offset = getBitOffset(num);

    uint8_t mask = (1 << k) - 1;

    //check if data spans across buckets

    if (offset + k > 8) {
        unsigned nextIndex = idx + 1;
        unsigned shiftInNextBucket = offset + k - 8; // how many bits spill over to the next bucket
        unsigned valueInFirstBucket = buckets[idx] >> offset; //extract the bits from the curr bucket
        unsigned valueInSecondBucket = buckets[nextIndex] & ((1 << shiftInNextBucket) - 1); //extract from next
        //combine the two values to form the full current value
        unsigned currentValue = valueInFirstBucket | (valueInSecondBucket << (8 - offset));
        if (currentValue < mask) { // if the max representation has not been reached yet
            currentValue++; //add
            buckets[idx] &= ~(mask >> (8 - offset)); //clear the current bits
            buckets[idx] |= currentValue << offset; //set the new value
            buckets[nextIndex] &= ~((1 << shiftInNextBucket) - 1); //clear the next bucket
            buckets[nextIndex] |= currentValue >> (8 - offset); // set the new value
            return true;
        }

    } else {

        //single bucket case <3333

        unsigned currValue = (buckets[idx] >> offset) & mask;

        if( currValue < mask) { //if the max has not been reached
            currValue++; //add
            buckets[idx] &= ~(mask << offset); //clear
            buckets[idx] |= (currValue << offset); //set
            return true;

        }

    }
    return false;

}

bool MultiSet::removeNumber(unsigned int num) {
    if (num > n) {
        return false;
    }

    unsigned idx = getIndex(num);
    unsigned offset = getBitOffset(num);

    uint8_t mask = (1 << k) - 1;

    //check if data spans across buckets

    if (offset + k > 8) {
        unsigned nextIndex = idx + 1;
        unsigned shiftInNextBucket = offset + k - 8; // how many bits spill over to the next bucket
        unsigned valueInFirstBucket = (buckets[idx] >> offset) & ((1 << (8 - offset)) - 1); //extract the bits from the curr bucket
        unsigned valueInSecondBucket = (buckets[nextIndex] & ((1 << shiftInNextBucket) - 1)); //extract from next
        //combine the two values to form the full current value
        unsigned currentValue = valueInFirstBucket | (valueInSecondBucket << (8 - offset));
        if (currentValue > 0) {
            currentValue--; //remove
            buckets[idx] &= ~(mask >> (8 - offset)); //clear the current bits
            buckets[idx] |= currentValue << offset; //set the new value
            buckets[nextIndex] &= ~((1 << shiftInNextBucket) - 1); //clear the next bucket
            buckets[nextIndex] |= currentValue >> (8 - offset); // set the new value
            return true;
        }

    } else {

        //single bucket case <3333

        unsigned currValue = (buckets[idx] >> offset) & mask;

        if( currValue > 0) {
            currValue--; //remove
            buckets[idx] &= ~(mask << offset); //clear
            buckets[idx] |= (currValue << offset); //set
            return true;

        }

    }
    return false;
}


unsigned MultiSet::getCountOccurrences (unsigned num) const {
    if (num > n) {
        return 0;
    }

    unsigned index = getIndex(num);
    unsigned offset = getBitOffset(num);
    unsigned mask = (1 << k) - 1;

    //check if spans across two buckets

    if (offset + k > 8) {
        unsigned nextIndex = index + 1;
        unsigned shiftInNextBucket = offset + k - 8;
        unsigned valueInFirstBucket = buckets[index] >> offset;
        unsigned valueInSecondBucket = buckets[nextIndex] & ((1 << shiftInNextBucket) - 1);
        return valueInFirstBucket | (valueInSecondBucket << (8 - offset));
    } else {
        return (buckets[index] >> offset) & mask;
    }
}

void MultiSet::printSet() const {
    std::cout << '{';
    bool first = true;
    for (unsigned i = 0; i <= n; ++i) {
        unsigned count = getCountOccurrences(i);
        for (unsigned j = 0; j < count; ++j) {
            if (!first) std::cout << ", ";
            std::cout << i;
            first = false;
        }
    }
    std::cout << '}';
}
