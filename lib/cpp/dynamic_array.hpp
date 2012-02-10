#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP
template<typename T>
class DynamicArray
{
public:
    enum copy_t { SAME, DEEP, ARG};
private:
    DynamicArray<T> *return_;
    size_t references_, length_;
    T *data_;
public:
    DynamicArray(int = 1);
    DynamicArray(const DynamicArray<T>&, copy_t = SAME);
    ~DynamicArray();
    void swap(DynamicArray<T>&);
    DynamicArray<T>& operator=(DynamicArray<T>);
    
    const T& get(size_t) const;
    void set(size_t, const T&);
    
    DynamicArray<T>& operator++();
    DynamicArray<T>& operator--();
}
#include "dynamic_array.ipp"
#endif