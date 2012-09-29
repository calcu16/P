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
template<typename T, typename... TS>
wrapper::Union<T, TS...>::Union()
    : active_(-1), value_(NULL)
{
    // Nothing else to do
}

template<typename T, typename... TS>
wrapper::Union<T, TS...>::Union(const Union<T, TS...>& copy)
    : active_(copy.active_), value_(copy.copyActive())
{
    // Nothing else to do
}

template<typename T, typename... TS>
wrapper::Union<T, TS...>::~Union()
{
    destroyActive();
}

template<typename T, typename... TS>
void wrapper::Union<T, TS...>::swap(Union<T,TS...>& other)
{
    std::swap(active_, other.active_);
    std::swap(value_, other.value_);
}

template<typename T, typename... TS>
wrapper::Union<T, TS...>&
    wrapper::Union<T, TS...>::operator=(Union<T,TS...> rhs)
{
    swap(rhs);
    return *this;
}

template<typename T, typename... TS>
template<int I>
typename wrapper::type<I, T, TS...>::const_reference
    wrapper::Union<T, TS...>::get() const
{
    assert(I == active_);
    return *((typename type<I,T,TS...>::const_pointer)value_);
}

template<typename T, typename... TS>
template<int I>
void wrapper::Union<T, TS...>::set
    (typename type<I, T, TS...>::const_reference value)
{
    void* pointer = (void*)(new (typename type<I,T,TS...>::value)(value));
    destroyActive();
    value_ = pointer;
    active_ = I;
}

template<typename T, typename... TS>
wrapper::Union<T, TS...>::operator int() const
{
    return active_;
}

template<typename T, typename... TS>
void* wrapper::Union<T, TS...>::copyActive() const
{
    return helper_.copy(active_, value_);
}

template<typename T, typename... TS>
void wrapper::Union<T, TS...>::destroyActive()
{
    helper_.copy(active_, value_);
}

template<typename T, typename... TS>
template<int I>
void* wrapper::Union<T, TS...>::Helper<I>::copy(int active, void* value) const
{
    assert(active == -1);
    assert(value == NULL);
    return NULL;
}

template<typename T, typename... TS>
template<int I>
void wrapper::Union<T, TS...>::Helper<I>::destroy(int active, void* value) const
{
    assert(active == -1);
    assert(value == NULL);
}

template<typename T, typename... TS>
template<int I, typename U, typename... US>
void* wrapper::Union<T, TS...>::
    Helper<I,U,US...>::copy(int active, void* value) const
{
    return active == I ? new U(*(U*)value) : next_.copy(active, value);
}

template<typename T, typename... TS>
template<int I, typename U, typename... US>
void wrapper::Union<T,TS...>::
    Helper<I,U,US...>::destroy(int active,void* value) const
{
    if(active == I)
        delete (U*)value;
    else
        next_.destory(active, value);
}

