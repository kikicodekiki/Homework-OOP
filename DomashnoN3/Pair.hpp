#pragma once
#include <iostream>

template <typename T, typename D>
class Pair {
public:

    Pair() = default;
    Pair (const T& newFirst, const D& newSecond);
    Pair (T&& newFirst, D&& newSecond);

    void setFirst(const T& other);
    void setFirst (T&& other);
    void setSecond(const D& other);
    void setSecond (D&& other);

    const T& getFirst () const;
    const D& getSecond () const;

private:

    T first;
    D second;

};



template <typename T, typename D>
Pair<T,D> ::Pair(const T& newFirst, const D& newSecond) {
    setFirst(newFirst);
    setSecond(newSecond);
}

template <typename T, typename D>
Pair<T,D> ::Pair(T&& newFirst, D&& newSecond) {
    setFirst(std::move(newFirst));
    setSecond(std::move(newSecond));
}

template <typename T, typename D>
void Pair<T,D> ::setFirst(const T& other) {
    first = other;
}

template <typename T, typename D>
void Pair<T,D> ::setFirst(T&& other) {
    first = std::move(other);
}

template <typename T, typename D>
void Pair<T,D> :: setSecond(const D& other) {
    second = other;
}

template <typename T, typename D>
void Pair<T,D> :: setSecond (D&& other) {
    second = std::move(other);
}

template <typename T, typename D>
const T& Pair<T,D> :: getFirst () const {
    return first;
}
template <typename T, typename D>
const D& Pair<T,D> :: getSecond () const {
    return second;
}



