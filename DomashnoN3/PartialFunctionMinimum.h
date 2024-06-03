#pragma once
#include "PartialFunctionContainer.h"
#include "ContainerHolder.h"

class PartialFunctionMinimum : public  ContainerHolder {
public:
    PartialFunctionMinimum (const PartialFunctionContainer& funcs);

    bool isDefined (int32_t point) const override;
    int32_t operator()(int32_t point) const override;
    PartialFunction* clone() const override;

private:
    PartialFunctionContainer container;
};
