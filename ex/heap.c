#include <stdlib.h>
#include <stdio.h>

#define out (*ret)
#define CHECK_ARGS(t) (ret = ret == NULL ? malloc(sizeof(t)) : ret)
#define GET_RETURN(r) (&(r))

typedef int (*compare_t)(void * const,void * const);

typedef struct heap heap;
struct heap
{
    void** items_;
    compare_t compare_;
    int capacity_, length_;
};

static void extend(void ** const in, int const length, int const capacity, void ** out)
{
    int i;
    CHECK_ARGS(void**);
    out = malloc(sizeof(void*) * capacity);
    for(i = 0; i < length; ++i)
        out[i] = in[i];
}
static void contract(void ** const in, int const length, void ** out)
{
    int i;
    CHECK_ARGS(void**);
    out = malloc(sizeof(void*) * length);
    for(i = 0; i < length; ++i)
        out[i] = in[i];
}

static void sift_up(compare_t const compare, void** const in, int const index, void ** out)
{
    int parent;
    void* temp;
    CHECK_ARGS(void**);
    out = in;
    parent = (index - 1) >> 1;
    if(index == 0 || compare(out[index], out[parent]) >= 0)
        return;
    temp = out[index];
    out[index] = out[parent];
    out[parent] = temp;
    sift_up(compare, out, parent, GET_RETURN(out));
}

static void sift_down(compare_t const compare, void** const in, int const index, int const length, void ** out)
{
    int lchild, rchild, min_child;
    void* temp;
    CHECK_ARGS(void**);
    out = in;
    min_child = (index << 1) + 1;
    if(min_child + 1 < length && compare(in[min_child], in[min_child+1]) > 0)
        min_child++;
    if(min_child >= length || compare(in[min_child], in[index]) >= 0)
        return;
    temp = out[index];
    out[index] = out[min_child];
    out[min_child] = temp;
    sift_down(compare, out, min_child, length, GET_RETURN(out));
}

heap* new_heap(compare_t const compare, heap out)
{
    CHECK_ARGS(heap);
    out.compare_ = compare;
    out.items_ = malloc(sizeof(void*));
    out.capacity_ = 1;
    out.length_ = 0;
    return &out;
}

void insert(heap const in, void* const item, heap out)
{
    CHECK_ARGS(heap);
    out = in;
    if(out.length_ + 1 > out.capacity_)
    {
        out.capacity_ <<= 1;
        extend(out.items_, out.length_, out.capacity_, GET_RETURN(out.items_));
    }
    out.items_[out.length_] = item;
    sift_up(out.compare_, out.items_, out.length_ , GET_RETURN(out.items_));
    out.length_++;
}

void* delete_min(heap const in, heap out)
{
    void *min;
    CHECK_ARGS(heap);
    out = in;
    min = out.items_[0];
    out.length_--;
    out.items_[0] = out.items_[out.length_];
    sift_down(out.compare_, out.items_, 0, out.length_, GET_RETURN(out.items_));
    if(out.length_ && (out.length_ << 2) <= out.capacity_)
        contract(out.items_, out.length_, GET_RETURN(out.items_));
}

void print_heap(heap const in)
{
    int i;
    for(i = 0; i < in.length_; ++i)
        printf("%d ", *(int*)in.items_[i]);
    printf("\n");
}


int int_compare(void * const left,void * const right)
{
    return *((int*)left) - *((int*)right);
}

int main(void)
{
    int numbers[10] = {0,1,2,3,4,5,6,7,8,9};
    heap start = *new_heap(int_compare, NULL);
    insert(start, numbers + 5, GET_RETURN(start));
    print_heap(start);
    insert(start, numbers + 2, GET_RETURN(start));
    print_heap(start);
    insert(start, numbers + 7, GET_RETURN(start));
    print_heap(start);
    delete_min(start, GET_RETURN(start));
    print_heap(start);
    return 0;
}