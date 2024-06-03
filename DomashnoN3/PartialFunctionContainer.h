#pragma once
#include "PolymorphicPtr.h"
#include "MyVector.h"
#include "PartialFunction.h"

class PartialFunctionContainer {
public:
    void add (PartialFunction* func);
    void add (const PartialFunction& func);

    PolymorphicPtr<PartialFunction>& operator[] (size_t index);
    const PolymorphicPtr<PartialFunction>& operator[] (size_t index) const;

    void execute(size_t index, void (*func)(PartialFunction*));
    void execute (size_t index, void (*func)(const PartialFunction*)) const;

    size_t getSize() const;
private:
    MyVector<PolymorphicPtr<PartialFunction>> data;

};
