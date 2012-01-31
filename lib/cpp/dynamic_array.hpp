template<typename T, typename P>
class DynamicArray
{
public:
    enum copy_t { DEEP, ARG };
private:
    P *parent_;
    DynamicArray<T> *return_;
    size_t references_, length_;
    T *data_;
public:
    DynamicArray(int = 1, P* = NULL);
    DynamicArray(const DynamicArray&, copy_t = DEEP);
    void swap(DynamicArray&);
    DynamicArray& operator=(DynamicArray);
    
    const T& get(size_t index) const;
    void set(size_t index, const T& ref);
}
