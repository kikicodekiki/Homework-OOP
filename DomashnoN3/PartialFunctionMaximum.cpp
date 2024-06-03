#include "PartialFunctionMaximum.h"


PartialFunctionMaximum ::PartialFunctionMaximum(const PartialFunctionContainer &funcs) : ContainerHolder(container)
{}


bool PartialFunctionMaximum::isDefined(int32_t point) const {
    size_t size = container.getSize();

    for (size_t i = 0; i < size; i++) {
        if (!container[i] ->isDefined(point)) {
            return false;
        }
    }
    return true;
}

int32_t PartialFunctionMaximum::operator()(int32_t point) const {
    if (!isDefined(point)) {
        throw std::runtime_error("point not defined"); //maybe not the best exception but idk
    }

    size_t size = container.getSize();

    int32_t maximum = container[0]->operator()(point);

    for (size_t i = 0; i < size; i++) {

        int32_t temp = container[i]->operator()(point);

        if (temp > maximum) {
            maximum = temp;
        }
    }

    return maximum;
}

PartialFunction* PartialFunctionMaximum::clone() const {
    return new PartialFunctionMaximum(*this);
}
