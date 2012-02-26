#ifndef FIXED_ARRAY_HPP
#define FIXED_ARRAY_HPP
#include <cstddef>
template<typename T>
class FatPointer;

template<typename T, size_t I>
class FixedArray
{
private:
    T data_[I];
    friend class FatPointer<T>;
public:
    T& operator[](size_t index) { return data_[index]; }
};
#endif
