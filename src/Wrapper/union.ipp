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
wrapper::Union<T, TS...>& wrapper::Union<T, TS...>::operator=(Union<T,TS...> rhs)
{
    swap(rhs);
    return *this;
}

template<typename T, typename... TS>
template<int I>
typename wrapper::type<I, T, TS...>::const_reference
    wrapper::Union<T, TS...>::get()
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
void* wrapper::Union<T, TS...>::Helper<I,U,US...>::copy(int active, void* value) const
{
    return active == I ? new U(*(U*)value) : next_.copy(active, value);
}

template<typename T, typename... TS>
template<int I, typename U, typename... US>
void wrapper::Union<T,TS...>::Helper<I,U,US...>::destroy(int active,void* value) const
{
    if(active == I)
        delete (U*)value;
    else
        next_.destory(active, value);
}

