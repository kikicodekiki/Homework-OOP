#include "PartialFunctionMinimum.h"
#include "PartialFunctionMaximum.h"
#include "PartialFunctionFactory.h"
#include <iostream>
#include "PartialFunctionContainer.h"
#include "PartialFunctionByCriteria.h"
#include "FirstCriteria.h"
#include "SecondCriteria.h"
#include "ThirdCriteria.h"

PartialFunction* PartialFunctionFactory::createFunctionFromFile(const MyString &fileName) {

    std::ifstream myFile (fileName.c_str(), std::ios::binary);

    if(!myFile.is_open()) {
        throw std::runtime_error("file not open");
    }

    return constructFromBinary(myFile);
}

PartialFunction* PartialFunctionFactory::constructFromBinary(std::ifstream &ifs) {
    int16_t N, T;

    static int maxN = 32;

    static int first = 0, second = 1, third = 2, fourth = 3, fifth = 4;

    ifs.read ( (char*)&N, sizeof(N) );
    ifs.read( (char*)&T, sizeof(T) );

    if ( N <= 0 || N > maxN ) {
        throw std::out_of_range ("N should be between 0 and 32");
    }

    if ( T == first ) {
        return firstCriteria(ifs, N);
    }

    if (T == second) {
        return secondCriteria(ifs, N);
    }

    if (T == third) {
        return thirdCriteria(ifs, N);
    }

    if (T == fourth) {
        return handleFourthCase(ifs, N);
    }

    if (T == fifth) {
        return handleFifthCase(ifs, N);
    }

    throw std::invalid_argument ("T is not valid");

}

PartialFunction* PartialFunctionFactory::handleFourthCase (std::ifstream& ifs, int16_t N) {
    MyString filePaths[N];
    PartialFunctionContainer container;

    for (size_t i = 0; i < N; i++) {
        char ch;
        while (ifs.read((char*)&ch, sizeof(ch)) && ch !='\0') {
           MyString result(&ch);
           filePaths[i] += result;
        }
        container.add(createFunctionFromFile(filePaths[i].c_str()));

    }

    return new PartialFunctionMaximum(container);

}

PartialFunction* PartialFunctionFactory::handleFifthCase (std::ifstream& ifs, int16_t N) {
    MyString filePaths[N];
    PartialFunctionContainer container;

    for (size_t i = 0; i < N; i++) {
        char ch;
        while (ifs.read((char*)&ch, sizeof(ch)) && ch !='\0') {
            MyString result(&ch);
            filePaths[i] += result;
        }
        container.add(createFunctionFromFile(filePaths[i].c_str()));

    }

    return new PartialFunctionMinimum(container);
}

PartialFunction* PartialFunctionFactory::firstCriteria (std::ifstream& ifs, int16_t N ) {
    int32_t args[N];
    int32_t results[N];

    ifs.read((char*) args, sizeof(int32_t) * N);
    ifs.read((char*)results, sizeof(int32_t) * N);

    FirstCriteria func(args, results, N);

    return new PartialFunctionByCriteria<FirstCriteria>(func);
}

PartialFunction* PartialFunctionFactory::secondCriteria (std::ifstream& ifs, int16_t N ) {
    int32_t args[N];

    ifs.read((char*)args, sizeof(int32_t) * N);

    SecondCriteria func (args, N);

    return new PartialFunctionByCriteria<SecondCriteria>(func);
}

PartialFunction* PartialFunctionFactory::thirdCriteria (std::ifstream& ifs, int16_t N ) {
    int32_t args[N];

    ifs.read((char*) args, sizeof(int32_t) * N);

    ThirdCriteria func (args, N);

    return new PartialFunctionByCriteria<ThirdCriteria>(func);
}
