#pragma once
#include <iostream>

class PartialFunction {
public:
    virtual bool isDefined (int32_t point ) const = 0;
    virtual int32_t operator() (int32_t) const  = 0;
    virtual PartialFunction* clone() const = 0;
    virtual ~PartialFunction() = default;

};
