template<typename S, typename P>
wrapper::ConstIterator<S,P>::ConstIterator(const typename S::const_iterator& candy)
    : iterator_ (candy)
{
}

template<typename S, typename P>
wrapper::ConstIterator<S,P>& wrapper::ConstIterator<S,P>::operator++()
{
    iterator_++;
    return *this;
}

template<typename S, typename P>
const P& wrapper::ConstIterator<S,P>::operator*() const
{
    return **iterator_;
}
template<typename S, typename P>
const P* wrapper::ConstIterator<S,P>::operator->() const
{
    return &(**iterator_);
}

template<typename S, typename P>
bool wrapper::ConstIterator<S,P>::operator==(const wrapper::ConstIterator<S,P>& rhs) const
{
    return iterator_ == rhs.iterator_;
}

template<typename S, typename P>
bool wrapper::ConstIterator<S,P>::operator!=(const wrapper::ConstIterator<S,P>& rhs) const
{
    return iterator_ != rhs.iterator_;
}
