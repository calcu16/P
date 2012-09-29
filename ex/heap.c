/*
Copyright (c) 2012, Andrew Carter, Dietrich Langenbach, Xanda Schofield
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/
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
