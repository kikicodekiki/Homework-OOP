#include "PartialFunctionContainer.h"

void PartialFunctionContainer::add(PartialFunction *func) {
    data.pushBack(PolymorphicPtr<PartialFunction>(func));
}

void PartialFunctionContainer::add(const PartialFunction &func) {
    data.pushBack(PolymorphicPtr<PartialFunction>(func.clone()));
}

PolymorphicPtr<PartialFunction>& PartialFunctionContainer::operator[](size_t index) {
    return data[index];
}

const PolymorphicPtr<PartialFunction>& PartialFunctionContainer::operator[](size_t index) const {
    return data[index];
}

void PartialFunctionContainer::execute(size_t index, void (*func)(PartialFunction *)) {
    func(data[index].get());
}

void PartialFunctionContainer::execute(size_t index, void (*func)(const PartialFunction *)) const {
    func(data[index].get());
}

size_t PartialFunctionContainer::getSize() const {
    return data.getSize();
}
