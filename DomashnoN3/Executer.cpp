#include "Executer.h"
#include "PartialFunction.h"
#include "PartialFunctionFactory.h"
#include "myString.h"
#include <iostream>

Executer::Executer(const MyString &fileName) {
    try {
        func = PartialFunctionFactory::createFunctionFromFile(fileName);
    } catch(const std::exception& e) {
        std::cerr << "Failed to create function " << e.what();
        func = nullptr;
    }
}

Executer::~Executer() {
    free();
}

void Executer::executeOverInterval(int32_t a, int32_t b) const {
    if (!func) {
        std::cerr << "Function not initialized";
        return;
    }

    for (int32_t i = a; i <= b; i++) {
        if (func->isDefined(i)) {
            std::cout << "f(" << i << ") = " << func->operator()(i) << std::endl;
        } else {
            std::cout << "f(" << i << ") is not defined.\n" ;
        }

    }
}

