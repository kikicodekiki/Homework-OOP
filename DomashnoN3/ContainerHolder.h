#pragma once
#include "PartialFunctionContainer.h"
#include "PartialFunction.h"

class ContainerHolder : public PartialFunction {
public:
    ContainerHolder(const PartialFunctionContainer& container) : container(container){}
private:
    PartialFunctionContainer container;
};
