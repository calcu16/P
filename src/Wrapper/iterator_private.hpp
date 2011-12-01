template<typename S, typename P>
wrapper::Iterator<S,P>::Iterator(const typename S::iterator& candy)
    : iterator_ (candy)
{
}

template<typename S, typename P>
wrapper::Iterator<S,P>& wrapper::Iterator<S,P>::operator++()
{
    iterator_++;
    return *this;
}

template<typename S, typename P>
P& wrapper::Iterator<S,P>::operator*() const
{
    return **iterator_;
}

template<typename S, typename P>
P* wrapper::Iterator<S,P>::operator->() const
{
    return &**iterator_;
}

template<typename S, typename P>
bool wrapper::Iterator<S,P>::operator==(const wrapper::Iterator<S,P>& rhs) const
{
    return iterator_ == rhs.iterator_;
}

template<typename S, typename P>
bool wrapper::Iterator<S,P>::operator!=(const wrapper::Iterator<S,P>& rhs) const
{
    return iterator_ != rhs.iterator_;
}