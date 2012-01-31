template<typename T, int N>
class FixedArray
{
private:
    FixedArray<T, N> return_;
    size_t references_;
    T data_[N];
public:
    const T& get(size_t index) const;
    void set(size_t index, const T& ref);
}
