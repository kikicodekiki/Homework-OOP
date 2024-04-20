#pragma once
#include <iostream>

class MultiSet {
    uint8_t* buckets;
    unsigned n;
    unsigned k; //the number of bits needed to represent the number

    unsigned bucketSize; //the number of elements in the array

    unsigned getIndex (unsigned num) const;
    unsigned getBitOffset(unsigned num) const;

    void free();
    void copyFrom (const MultiSet& other);

    bool addAcrossTwoBuckets (unsigned num);


public:

    static const int MIN_NUMBER_BITS = 1;
    static const int MAX_NUMBER_BITS = 8;

    MultiSet();
    MultiSet(unsigned num, unsigned maxOccurences );
    MultiSet(const MultiSet& other);
    MultiSet& operator= (const MultiSet& other);
    ~MultiSet();

    unsigned getBucketSize () const;
    unsigned getN () const;
    unsigned getK () const;
    uint8_t getMask () const;

    bool addNumber (unsigned num);
    bool removeNumber (unsigned num);
    unsigned getCountOccurrences (unsigned num) const;

    void printSet() const;
    void printBinary() const;


    void serialize (const char* fileName) const;

    void deserialize (const char* fileName);

    void addition () ;
    MultiSet complement() const;

};

MultiSet intersect (const MultiSet& lhs, const MultiSet& rhs);
MultiSet difference (const MultiSet& lhs, const MultiSet& rhs);
