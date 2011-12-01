#ifndef WRAPPER_ITERATOR_HPP
#define WRAPPER_ITERATOR_HPP
namespace wrapper
{
    template<typename S, typename P>
    class Iterator
    {
    private:
        typename S::iterator iterator_;
    public:
        Iterator(const typename S::iterator&);
        Iterator<S,P>& operator++();
        P& operator*() const;
        P* operator->() const;
        bool operator==(const Iterator<S,P>&) const;
        bool operator!=(const Iterator<S,P>&) const;
    };
}
#include "iterator_private.hpp"
#endif
