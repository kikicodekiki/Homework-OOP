#include <iostream>
#include "MultiSet.h"
#pragma once
#include <fstream>

const int MultiSet :: MIN_NUMBER_BITS;
const int MultiSet :: MAX_NUMBER_BITS;

unsigned MultiSet::getIndex (unsigned num) const {
    return (num*k) / MAX_NUMBER_BITS;
}

unsigned MultiSet::getBitOffset(unsigned int num) const {
    return (num * k) % MAX_NUMBER_BITS;
}

unsigned MultiSet :: getBucketSize () const {
    return (n * k + 7) / MAX_NUMBER_BITS + 1 ; // +1 for 2k+1 case
}

unsigned MultiSet::getN() const {
    return n;
}

unsigned MultiSet::getK() const {
    return k;
}

uint8_t MultiSet :: getMask() const {
    uint8_t mask = (1 << k) - 1;
    return mask;
}

MultiSet ::MultiSet() : n(0), k(0), bucketSize(0), buckets(nullptr)
{ }

MultiSet ::MultiSet(unsigned n, unsigned k) : n(n) {
    if (k > MAX_NUMBER_BITS || k < MIN_NUMBER_BITS) {
        this -> k = MAX_NUMBER_BITS;
    } else {
        this -> k = k;
    }

    bucketSize = getBucketSize();

    buckets = new uint8_t [bucketSize] {0};
}

void MultiSet::free() {
    delete[] buckets;
    buckets = nullptr;
    n = 0;
    k = 0;
    bucketSize = 0;
}

void MultiSet::copyFrom (const MultiSet& other) {

    n = other.getN();

    k = other.getK();

    bucketSize = other.getBucketSize();

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

bool MultiSet::addAcrossTwoBuckets(unsigned int num) {
    unsigned idx = getIndex(num);
    unsigned offset = getBitOffset(num);

    uint8_t mask = getMask();

    unsigned nextIndex = idx + 1;
    unsigned shiftInNextBucket = offset + k - MAX_NUMBER_BITS; // how many bits spill over to the next bucket
    unsigned valueInFirstBucket = (buckets[idx] >> offset) & ((1 << (8 - offset)) - 1); //extract the bits from the curr bucket
    unsigned valueInSecondBucket = (buckets[nextIndex] & ((1 << shiftInNextBucket) - 1)); //extract from next
    //combine the two values to form the full current value
    unsigned currentValue = valueInFirstBucket | (valueInSecondBucket << (8 - offset));
    if (currentValue < mask) { // if the max representation has not been reached yet
        currentValue++; //add
        buckets[idx] &= ~(mask << offset); //clear the current bits
        buckets[idx] |= (currentValue << offset) & ((1 << 8) - 1);  // set lower part in the first bucket
        buckets[nextIndex] &= ~((1 << shiftInNextBucket) - 1); //clear the next bucket
        buckets[nextIndex] |= (currentValue >> (8 - offset)); // set the new value
        return true;
    } else {
        return false; //no more space
    }

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

        return addAcrossTwoBuckets(num);

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

unsigned MultiSet::getCountOccurrences (unsigned num) const {
    if (num > n) {
        return 0;
    }

    unsigned index = getIndex(num);
    unsigned offset = getBitOffset(num);
    unsigned mask = (1 << k) - 1;

    //check if it spans across two buckets

    if (offset + k > 8) {
        unsigned nextIndex = index + 1;
        unsigned shiftInNextBucket = offset + k - MAX_NUMBER_BITS;
        unsigned valueInFirstBucket = buckets[index] >> offset;
        unsigned valueInSecondBucket = buckets[nextIndex] & ((1 << shiftInNextBucket) - 1);
        return valueInFirstBucket | (valueInSecondBucket << (MAX_NUMBER_BITS - offset));
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
    std::cout << '}' << std::endl;
}

void MultiSet::printBinary() const {
    for (int i = 0; i < bucketSize; ++i) {

        std::cout << "Bucket " << i << ": ";

        for (int  j = 7; j >= 0; j--) {  // print each bit in the byte
            std::cout << ((buckets[i] & (1 << j)) ? '1' : '0');
        }

        std::cout << std::endl;
    }
}

void MultiSet :: serialize (const char* fileName) const {
    std::ofstream ofs (fileName, std::ios::out | std::ios::binary);

    if (!ofs.is_open()) {
        std::cerr << "File " << fileName << "not open";
        return;
    }

    ofs.write((const char*)&n, sizeof(n));
    ofs.write((const char*)&k, sizeof(k));

    ofs.write ((const char*)buckets, bucketSize);


    ofs.close();
}

void MultiSet::deserialize(const char *fileName) {
    std::ifstream ifs (fileName, std::ios::in | std::ios::binary);

    if (!ifs.is_open()) {
        std::cerr << "File " << fileName << "not open";
        return;
    }

    ifs.read((char*)&n, sizeof(n));
    ifs.read((char*)&k, sizeof(k));

    bucketSize = getBucketSize();

    delete[] buckets;
    buckets = new uint8_t [bucketSize] {0};

    ifs.read((char *) buckets, bucketSize);

    ifs.close();

}

MultiSet intersect (const MultiSet& lhs, const MultiSet& rhs) {

    unsigned minN = std::min (lhs.getN(), rhs.getN());
    unsigned minK = std::min(lhs.getK(), rhs.getK());

    MultiSet result (minN, minK);

    for(unsigned i = 0; i <= minN; i++) {
        unsigned occurrencesLhs = lhs.getCountOccurrences(i);
        unsigned occurrencesRhs = rhs.getCountOccurrences(i);

        unsigned occurrences = std::min(occurrencesLhs, occurrencesRhs);

        for (unsigned j = 0; j < occurrences; j++) {
            result.addNumber(i);
        }
    }

    return result;
}

MultiSet difference (const MultiSet& lhs, const MultiSet& rhs) {

    int maxN = std::max(lhs.getN(), rhs.getN());

    MultiSet result (maxN, lhs.getK());

    for (unsigned i = 0; i <= maxN; i++) {
        unsigned occurrencesLhs = lhs.getCountOccurrences(i);
        unsigned occurrencesRhs = rhs.getCountOccurrences(i);
        if (occurrencesLhs > occurrencesRhs) {
            for (unsigned j = 0; j < (occurrencesLhs - occurrencesRhs); j++) {
                result.addNumber(i);
            }
        }
    }

    return result;
}

MultiSet MultiSet::complement() const {
    unsigned maxValue = getMask();  // 2^k - 1
    MultiSet result(n, k);

    for (unsigned i = 0; i <= n; i++) {
        unsigned currentCount = getCountOccurrences(i);  // get the current count of the element
        unsigned newCount = maxValue - currentCount;  // calculate the complement count

        unsigned idx = result.getIndex(i);
        unsigned offset = result.getBitOffset(i);

        if (offset + k > MAX_NUMBER_BITS) { //if data spans across two buckets

            unsigned shiftInNextBucket = offset + k - MAX_NUMBER_BITS;
            uint8_t maskFirstBucket = (1 << (MAX_NUMBER_BITS - offset)) - 1;
            uint8_t maskSecondBucket = (1 << shiftInNextBucket) - 1;

            result.buckets[idx] &= ~(maskFirstBucket << offset); //set first bucket
            result.buckets[idx] |= (newCount << offset);

            result.buckets[idx + 1] &= ~maskSecondBucket; //set second bucket
            result.buckets[idx + 1] |= (newCount >> (MAX_NUMBER_BITS - offset));

        } else {
            // if the data fits within a single bucket
            uint8_t mask = ((1 << k) - 1) << offset;
            result.buckets[idx] &= ~mask;
            result.buckets[idx] |= (newCount << offset);
        }
    }

    return result;
}


// lol thought that we needed a remove function as well so here it is cus
// I worked too hard on it not to at least show it off
// ;)

/*bool MultiSet::removeNumber(unsigned int num) {
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
*/



