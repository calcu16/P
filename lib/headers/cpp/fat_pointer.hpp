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
#ifndef FAT_POINTER_HPP
#define FAT_POINTER_HPP
#include <cstddef>
template<typename T, size_t I>
class FixedArray;

template<typename T>
class FatPointer;

template<typename T>
struct PointerType
{
    typedef T *type;
};

template<typename T>
struct PointerType<FatPointer<T> >
{
    typedef typename FatPointer<T>::type *type;
};

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
    typedef T type;
    FatPointer();
    FatPointer(const T&, size_t = 1);
    FatPointer(const FatPointer<T>&, bool = false, int = 0);
    FatPointer(typename PointerType<T>::type);
    template<size_t I>
    FatPointer(const FixedArray<T, I>&, int = 0);
    ~FatPointer();
    
    void swap(FatPointer<T>&);
    FatPointer<T>& operator=(const FatPointer<T>&);
    FatPointer<T>& operator=(void*);
    FatPointer<T>& operator=(typename PointerType<T>::type);
    
    T& operator*();
    T* operator->();
    T& operator[](const int);
    
    FatPointer<FatPointer<T> > operator&();
    
    FatPointer<T> operator+(const int) const;
    
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
FatPointer<T> operator+(const int, const FatPointer<T>&);
template<typename T>
FatPointer<T> operator-(const int, const FatPointer<T>&);
/*
template<typename T>
T& operator[](const int, FatPointer<T>);
*/
#include "fat_pointer.ipp"
#endif
