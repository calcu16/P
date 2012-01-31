#ifndef WRAPPER_ITERATOR_HPP
#define WRAPPER_ITERATOR_HPP
namespace wrapper
{
    template<typename I, typename P>
    class Iterator
    {
    private:
        I iterator_;
    public:
        Iterator(const I&);
        Iterator<I,P>& operator++();
        Iterator<I,P> operator++(int);
        Iterator<I,P>& operator--();
        Iterator<I,P> operator--(int);
        P& operator*() const;
        P* operator->() const;
        bool operator==(const Iterator<I,P>&) const;
        bool operator!=(const Iterator<I,P>&) const;
    };
}
#include "iterator.ipp"
#endif
