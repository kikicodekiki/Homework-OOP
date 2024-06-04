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



void Executer::userRequestedExecution() const {
    if (!func) {
        std::cerr << "Function not initialized";
        return;
    }

    int32_t current = 0;
    bool initialized = false;

    while (true) {
        std::cout << "Do you want the next point? (enter 'yes' or 'no') " << std::endl;
        MyString str;
        std::cin >> str;

        static MyString yes = "yes";
        static MyString no = "no";

        if (str == no) {
            break;
        } else if (str == yes) {
            if (!initialized) {
                std::cout << "Enter the starting point: ";
                std::cin >> current;
                initialized = true;
            } else {
                ++current;
            }

            if (func->isDefined(current)) {
                std::cout << "f(" << current << ") = " << func->operator()(current) << std::endl;
            } else {
                std::cout << "f(" << current << ") is not defined." << std::endl;
            }
        } else {
            std::cout << "Invalid input. Please enter 'yes' or 'no'." << std::endl;
        }
    }

}

void Executer::free() {
    delete func;
}

Executer::Executer(const char *fileName) {
    try {
        func = PartialFunctionFactory::createFunctionFromFile(fileName);
    } catch(const std::exception& e) {
        std::cerr << "Failed to create function " << e.what();
        func = nullptr;
    }
}

