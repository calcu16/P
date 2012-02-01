#ifndef FAT_POINTER_HPP
#define FAT_POINTER_HPP
template<typename T>
class FatPointer
{
private:
    FatPointer<FatPointer<T> > parent_;
    DynamicArray<T> *data_;
    size_t index_;
public:
    FatPoint()
}
#endif
