#ifndef FAT_POINTER_HPP
#define FAT_POINTER_HPP
#include <cstddef>

template<typename T>
T* getReference(const T&);

template<typename T>
class FatPointer
{
private:
    bool owner_;
    T *data_;
    size_t length_;
    int index_;
    
    FatPointer<T>* reference();
    friend T* getReference<T>(const T&);
public:
    FatPointer();
    FatPointer(T, size_t = 1);
    FatPointer(const FatPointer<T>&, bool = false, int delta = 0);
    ~FatPointer();
    
    void swap(FatPointer<T>&);
    FatPointer<T>& operator=(const FatPointer<T>&);
    FatPointer<T>& operator=(void*);
    
    T& operator*();
    T* operator->();
    T& operator[](const int);
    
    FatPointer<FatPointer<T> > operator&();
    
    FatPointer<T> operator+(const int);
    FatPointer<T> operator-(const int);
    
    FatPointer<T>& operator++();
    FatPointer<T> operator++(int);
    
    FatPointer<T>& operator--();
    FatPointer<T> operator--(int);
    
    bool operator==(const FatPointer<T>&);
    bool operator==(void*);
    
    template<typename U>
    bool operator==(const FatPointer<U>&);
    template<typename U>
    bool operator!=(const FatPointer<U>&);
    
    
    template<typename U> friend class FatPointer<T>;
};
template<typename T>
FatPointer<T> operator+(const int, FatPointer<T>);
template<typename T>
FatPointer<T> operator-(const int, FatPointer<T>);

FatPointer<char> __NULL__();

template<typename T>
class FatPointer<FatPointer<T> >;

/*
template<typename T>
T& operator[](const int, FatPointer<T>);
*/
#include "fat_pointer.ipp"
#endif
