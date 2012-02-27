#include "fat_pointer.hpp"
/// #include <stdlib.h>
#include <stdlib.h>
/// #include <stdio.h>
#include <stdio.h>


/// struct heap
struct struct_heap
{
///    int* items_;
    FatPointer<int> items_;
///    int capacity_, length_;
    int capacity_, length_;
};
/// typedef struct heap heap;
typedef struct_heap heap;

/// static void extend(int * const in, int const length, int const capacity, int * out)
struct extend_return { FatPointer<int> out; };
static extend_return extend(FatPointer<int> in, int length, int capacity)
{
    extend_return __return__;
    /// int i;
    int i;
    /// out = malloc(sizeof(int) * capacity);
    __return__.out = FatPointer<int>(0, capacity);
    /// for(i = 0; i < length; ++i)
    for(i = 0; i < length; ++i)
        /// out[i] = in[i];
        __return__.out[i] = in[i];
    ///
    return __return__;
}

/// static void contract(int * const in, int const length, int * out)
struct contract_return { FatPointer<int> out; };
static contract_return contract(FatPointer<int> in, int length)
{
    contract_return __return__;
    /// int i;
    int i;
    /// out = malloc(sizeof(int) * length);
    __return__.out = FatPointer<int>(0, length);
    /// for(i = 0; i < length; ++i)
    for(i = 0; i < length; ++i)
        /// out[i] = in[i];
        __return__.out[i] = in[i];
    ///
    return __return__;
}

/// static void sift_up(int * const in, int const index, int * out)
struct sift_up_return { FatPointer<int> out; };
static sift_up_return sift_up(FatPointer<int> in, int index)
{
    sift_up_return __return__;
    /// int parent;
    int parent;
    /// int temp;
    int temp;
    /// out = in;
    __return__.out = in;
    /// parent = (index - 1) >> 1;
    parent = (index - 1) >> 1;
    /// if(index == 0 || compare(out[index], out[parent]) >= 0)
    if(index == 0 || __return__.out[index] >= __return__.out[parent])
        /// return;
        return __return__;
    /// temp = out[index];
    temp = __return__.out[index];
    /// out[index] = out[parent];
    __return__.out[index] = __return__.out[parent];
    /// out[parent] = temp;
    __return__.out[parent] = temp;
    /// sift_up(out, parent, out);
    {
        sift_up_return __result__;
        __result__ = sift_up(__return__.out, parent);
        // tail call, could be optimized
        __return__.out = __result__.out;
    }
    return __return__;
}

/// static void sift_down(int * const in, int const index, int const length, int * out)
struct sift_down_return { FatPointer<int> out; };
static sift_down_return sift_down(FatPointer<int> in, int index, int length)
{
    sift_down_return __return__;
    /// int lchild, rchild, min_child;
    int lchild, rchild, min_child;
    /// int temp;
    int temp;
    /// out = in;
    __return__.out = in;
    /// min_child = (index << 1) + 1;
    min_child = (index << 1) + 1;
    /// if(min_child + 1 < length && compare(in[min_child], in[min_child+1]) > 0)
    if(min_child + 1 < length && in[min_child] > in[min_child+1])
        /// min_child++;
        min_child++;
    /// if(min_child >= length || in[min_child] >= in[index] >= 0)
    if(min_child >= length || in[min_child] >= in[index] >= 0)
    ///    return;
        return __return__;
    /// temp = out[index];
    temp = __return__.out[index];
    /// out[index] = out[min_child];
    __return__.out[index] = __return__.out[min_child];
    /// out[min_child] = temp;
    __return__.out[min_child] = temp;
    /// sift_down(out, min_child, length, out);
    {
        sift_down_return __result__;
        __result__ = sift_down(__result__.out, min_child, length);
        __return__.out = __result__.out;
    }
    return __return__;
}


/// heap* new_heap(heap out)
struct new_heap_return { FatPointer<heap> value; heap out; };
new_heap_return new_heap()
{
    new_heap_return __return__;
    /// out.items_ = malloc(sizeof(int));
    __return__.out.items_ = FatPointer<int>(0, 1);
    /// out.capacity_ = 1;
    __return__.out.capacity_ = 1;
    /// out.length_ = 0;
    __return__.out.length_ = 0;
    /// return &out;
    __return__.value = FatPointer<heap>(__return__.out);
    return __return__;
}

/// void insert(heap const in, int const item, heap out)
struct insert_return { heap out; };
insert_return insert(heap in, int item)
{
    insert_return __return__;
    /// out = in;
    __return__.out = in;
    /// if(out.length_ + 1 > out.capacity_)
    if(__return__.out.length_ + 1 > __return__.out.capacity_)
    {
        /// out.capacity_ <<= 1;
        __return__.out.capacity_ <<= 1;
        /// extend(out.items_, out.length_, out.capacity_, out.items_);
        {
            extend_return __result__;
            __result__ = extend(__return__.out.items_, __return__.out.length_, __return__.out.capacity_);
            __return__.out.items_ = __result__.out;
        }
    }
    /// out.items_[out.length_] = item;
    __return__.out.items_[__return__.out.length_] = item;
    /// sift_up(out.items_, out.length_ , out.items_);
    {
        sift_up_return __result__;
        __result__ = sift_up(__return__.out.items_, __return__.out.length_);
        __return__.out.items_ = __result__.out;
    }
    /// out.length_++;
    __return__.out.length_++;
    ///
    return __return__;
}


/// int delete_min(heap const in, heap out)
struct delete_min_return { int value; heap out; };
delete_min_return delete_min(heap in)
{
    delete_min_return __return__;
    /// int min;
    int min;
    /// out = in;
    __return__.out = in;
    /// min = out.items_[0];
    min = __return__.out.items_[0];
    /// out.length_--;
    __return__.out.length_--;
    /// out.items_[0] = out.items_[out.length_];
    __return__.out.items_[0] = __return__.out.items_[__return__.out.length_];
    /// sift_down(out.items_, 0, out.length_, out.items_);
    {
        sift_down_return __result__;
        __result__ = sift_down(__return__.out.items_, 0, __return__.out.length_);
        __return__.out.items_ = __result__.out;
    }
    /// if(out.length_ && (out.length_ << 2) <= out.capacity_)
    if(__return__.out.length_ && (__return__.out.length_ << 2) <= __return__.out.capacity_)
        /// contract(out.items_, out.length_, out.items_);
        {
            contract_return __result__;
            __result__ = contract(__return__.out.items_, __return__.out.length_);
            __return__.out.items_ = __result__.out;
        }
    /// return min;
    __return__.value = min;
    return __return__;
}

void print_heap(heap in)
{
    int i;
    for(i = 0; i < in.length_; ++i)
        printf("%d ", in.items_[i]);
    printf("\n");
}

int main(void)
{
    heap start = new_heap().out;
    start = insert(start, 5).out;
    print_heap(start);
    start = insert(start, 2).out;
    print_heap(start);
    start = insert(start, 7).out;
    print_heap(start);
    start = delete_min(start).out;
    print_heap(start);
    return 0;
}