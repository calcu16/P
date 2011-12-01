#ifndef WRAPPER_CONSTITERATOR_HPP
#define WRAPPER_CONSTITERATOR_HPP
namespace wrapper
{
    template<typename S, typename P>
    class ConstIterator
    {
    private:
        typename S::const_iterator iterator_;
    public:
        ConstIterator(const typename S::const_iterator&);
        ConstIterator<S,P>& operator++();
        const P& operator*() const;
        const P* operator->() const;
        bool operator==(const ConstIterator<S,P>&) const;
        bool operator!=(const ConstIterator<S,P>&) const;
    };
}
#include "constiterator_private.hpp"
#endif
