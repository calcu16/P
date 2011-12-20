template<typename I, typename P>
wrapper::Iterator<I,P>::Iterator(const I& candy)
    : iterator_ (candy)
{
}

template<typename I, typename P>
wrapper::Iterator<I,P>& wrapper::Iterator<I,P>::operator++()
{
    ++iterator_;
    return *this;
}

template<typename I, typename P>
wrapper::Iterator<I,P> wrapper::Iterator<I,P>::operator++(int)
{
    wrapper::Iterator<I,P> copy(iterator_);
    ++iterator_;
    return copy;
}

template<typename I, typename P>
wrapper::Iterator<I,P>& wrapper::Iterator<I,P>::operator--()
{
    --iterator_;
    return *this;
}

template<typename I, typename P>
wrapper::Iterator<I,P> wrapper::Iterator<I,P>::operator--(int)
{
    wrapper::Iterator<I,P> copy(iterator_);
    --iterator_;
    return copy;
}

template<typename I, typename P>
P& wrapper::Iterator<I,P>::operator*() const
{
    return **iterator_;
}

template<typename I, typename P>
P* wrapper::Iterator<I,P>::operator->() const
{
    return &**iterator_;
}

template<typename I, typename P>
bool wrapper::Iterator<I,P>::operator==(const wrapper::Iterator<I,P>& rhs) const
{
    return iterator_ == rhs.iterator_;
}

template<typename I, typename P>
bool wrapper::Iterator<I,P>::operator!=(const wrapper::Iterator<I,P>& rhs) const
{
    return iterator_ != rhs.iterator_;
}
