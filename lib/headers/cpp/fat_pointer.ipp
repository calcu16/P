/*
Copyright (c) 2012, Andrew Carter, Dietrich Langenbach, Xanda Schofield
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/
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
FatPointer<T>::FatPointer(typename PointerType<T>::type copy)
    : owner_(true), data_(copy == NULL ? NULL : new T(*copy)),
        length_(copy == NULL ? 0 : 1), index_(0)
{
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
    return *this;
}

template<typename T>
FatPointer<T>& FatPointer<T>::operator=(void* rhs)
{
    assert(rhs == NULL);
    FatPointer<T> copy;
    swap(copy);
    return *this;
}

template<typename T>
FatPointer<T>& FatPointer<T>::operator=(typename PointerType<T>::type rhs)
{
    FatPointer<T> copy(rhs);
    swap(copy);
    return *this;
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
