#pragma once
#include "PartialFunction.h"
#include "PartialFunctionContainer.h"
#include "myString.h"

class Executer {
public:
    Executer(const MyString& fileName);
    ~Executer();

    Executer(const Executer& other) = delete;
    Executer(Executer&& other) = delete;
    Executer& operator=(const Executer&) = delete;
    Executer& operator=(Executer&& ) = delete;

    void executeOverInterval (int32_t a, int32_t b) const;
    void userRequestedExecution() const;

private:
    PartialFunction* func;

    void free();

};
