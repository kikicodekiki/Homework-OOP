#pragma once
#include <iostream>

class MultiSet {
    uint8_t* buckets;
    unsigned n;
    unsigned k; //the number of bits needed to represent the number

    unsigned bucketSize;

    unsigned getIndex (unsigned num) const;
    unsigned getBitOffset(unsigned num) const;

    void free();
    void copyFrom (const MultiSet& other);


public:

    MultiSet();
    MultiSet(unsigned num, unsigned maxOccurences );
    MultiSet(const MultiSet& other);
    MultiSet& operator= (const MultiSet& other);
    ~MultiSet();

    bool addNumber (unsigned num);
    bool removeNumber (unsigned num);
    unsigned getCountOccurrences (unsigned num) const;

    void printSet() const;
    void printBinary() const;
#include <bitset> // For std::bitset for easy binary printing

    void printBucketsInBinary() const {
        std::cout << "Multiset Buckets in Binary:" << std::endl;
        for (unsigned i = 0; i < bucketSize; ++i) {
            // Convert each bucket's content to binary and print it
            std::cout << "Bucket " << i << ": " << std::bitset<8>(buckets[i]) << std::endl;
        }
    }


    void serialize (const char* fileName) const;

    void deserialize (const char* fileName) const;

friend MultiSet intersect (const MultiSet& lhs, const MultiSet& rhs);
friend MultiSet unify (const MultiSet& lhs, const MultiSet& rhs);

};, 

};
