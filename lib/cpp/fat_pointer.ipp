#include <algorithm>
#include <cassert>
template<typename T>
FatPointer<T>* FatPointer<T>::reference()
{
    return this;
}

template<typename T>
FatPointer<T>::FatPointer()
    : owner_(true), data_(NULL), length_(0), index_(0)
{
}

template<typename T>
FatPointer<T>::FatPointer(const T& t, size_t length)
    : owner_(true), data_(length == 0 ? NULL : new T[length]), length_(length), index_(0)
{
    for(size_t i = 0; i < length_; ++i)
        data_[i] = t;
}

template<typename T>
FatPointer<T>::FatPointer(const FatPointer<T>& copy, bool deep, int delta)
    : owner_(deep), data_(!deep ? copy.data_ : copy.length_ == 0 ? NULL : new T[copy.length_]),
        length_(copy.length_), index_(copy.index_ + delta)
{
    if(deep)
        for(size_t i = 0; i < length_; ++i)
            data_[i] = copy.data_[i];
}

template<typename T>
template<size_t I>
FatPointer<T>::FatPointer(const FixedArray<T, I>& copy, int delta)
    : owner_(false), data_(copy.data_), length_(I), index_(delta)
{
}

template<typename T>
FatPointer<T>::~FatPointer()
{
    if(owner_)
        delete [] data_;
}

template<typename T>
void FatPointer<T>::swap(FatPointer<T>& other)
{
    std::swap(owner_ , other.owner_ );
    std::swap(data_  , other.data_  );
    std::swap(length_, other.length_);
    std::swap(index_ , other.index_ );
}

template<typename T>
FatPointer<T>& FatPointer<T>::operator=(const FatPointer<T>& rhs)
{
    FatPointer<T> copy(rhs, true);
    swap(copy);
}

template<typename T>
FatPointer<T>& FatPointer<T>::operator=(void* rhs)
{
    assert(rhs == NULL);
    FatPointer<T> copy;
    swap(copy);
}

template<typename T>
T& FatPointer<T>::operator*()
{
    return (*this)[0];
}

template<typename T>
T* FatPointer<T>::operator->()
{
    return getReference(**this);
}

template<typename T>
T& FatPointer<T>::operator[](const int delta)
{
    return data_[index_ + delta];
}

template<typename T>
FatPointer<FatPointer<T> > FatPointer<T>::operator&()
{
    return FatPointer<T>(*this);
}

template<typename T>
FatPointer<T> FatPointer<T>::operator+(const int delta) const
{
    return FatPointer<T>(*this, false, delta);
}

template<typename T>
FatPointer<T>& FatPointer<T>::operator++()
{
    ++index_;
    return *this;
}

template<typename T>
FatPointer<T> FatPointer<T>::operator++(int)
{
    FatPointer<T> copy(*this);
    ++index_;
    return copy;
}

template<typename T>
FatPointer<T>& FatPointer<T>::operator--()
{
    --index_;
    return *this;
}

template<typename T>
FatPointer<T> FatPointer<T>::operator--(int)
{
    FatPointer<T> copy(*this);
    --index_;
    return copy;
}

template<typename T>
bool FatPointer<T>::operator==(void * null)
{
    return length_ == 0 && index_ == 0 && null == 0;
}

template<typename T>
bool FatPointer<T>::operator==(const FatPointer<T>& rhs)
{
    if(length_ != rhs.length_)
        return false;
    if(index_ != rhs.index_)
        return false;
    for(size_t i = 0; i < length_; ++i)
        if(data_[i] != rhs.data_[i])
            return false;
    return true;
}


template<typename T>
template<typename U>
bool FatPointer<T>::operator==(const FatPointer <U>& rhs)
{
    return length_ == 0 && rhs.length_ == 0 && index_ == rhs.index_;
}

template<typename T>
template<typename U>
bool FatPointer<T>::operator!=(const FatPointer <U>& rhs)
{
    return !(*this == rhs);
}

template<typename T>
FatPointer<T> operator+(const int delta, FatPointer<T> other)
{
    return other + delta;
}

template<typename T>
FatPointer<T> operator-(const int delta, FatPointer<T> other)
{
    return -delta + other;
}
/*
template<typename T>
T& operator[](const int delta, FatPointer<T> other)
{
    return other[delta];
}
*/

template<typename T>
FatPointer<T>* getReference(const FatPointer<T>& ref)
{
    return ref.reference();
}

template<typename T>
T* getReference(T& ref)
{
    return &ref;
}
