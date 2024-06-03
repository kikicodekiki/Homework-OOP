#pragma once
#include "PartialFunction.h"
#include "Pair.h"

template <typename T>
class PartialFunctionByCriteria : public PartialFunction {
    T myFunc;

public :
    bool isDefined (int32_t point ) const override;
    int32_t operator() (int32_t) const override;
    PartialFunction* clone() const override;

    PartialFunctionByCriteria(const T& func);

};

template <class T>
bool PartialFunctionByCriteria<T>::isDefined(int32_t point) const {
    Pair<bool,int> result = myFunc(point);
    return result.getFirst();
}

template <class T>
int32_t PartialFunctionByCriteria<T>::operator()(int32_t point) const {
    Pair<bool,int> result = myFunc(point);

    if (!result.getFirst()) {
        throw std::exception();
    }

   return result.getSecond();
}

template <class T>
PartialFunction* PartialFunctionByCriteria<T> ::clone() const {
    return new PartialFunctionByCriteria<T> (*this);
}

template <class T>
PartialFunctionByCriteria<T>::PartialFunctionByCriteria(const T &func) : myFunc(func) {}
