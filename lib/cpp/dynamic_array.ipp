template<typename T>
DynamicArray<T>::DynamicArray(int length)
    :   return_(NULL),
        references_(1),
        length_(length),
        data_(new T[length])
{
}

template<typename T>
DynamicArray<T>::DynamicArray(const DynamicArray& copy, copy_t type)
    :   return_((type == SAME ? copy.type_ : type) == DEEP ? NULL : copy),
        references_(1),
        length_(copy.length_),
        data_(new T[length_])
{
    if(return_ != NULL)
        ++*return_;

    for(size_t i = 0; i < length; ++i)
        data_[i] = copy.data_[i];
}

template<typename T>
DynamicArray<T>::~DynamicArray()
{
    delete data;
    if(return_ != NULL)
        --return_;
}

template<typename T>
void DynamicArray<T>::swap(DynamicArray<T>& other)
{
    std::swap(return_, other.return_);
    std::swap(references_, other.references_);
    std::swap(length_, other.length_);
    std::swap(data_, other.data_);
}

template<typename T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray<T> rhs)
{
    swap(rhs);
}

template<typename T>
const T& DynamicArray<T>::get(size_t index) const
{
    return data[index];
}

template<typename T>
void set(size_t index, const T& ref)
{
    data[index] = ref;
    if(return_ != NULL)
        return_->set(size_t, ref);
}

template<typename T>
DynamicArray<T>& operator++()
{
    ++references_;
}

template<typename T>
DynamicArray<T>& operator--()
{
    if((--references_) == 0 && return_ != NULL)
        --return_;
    delete this;
}
