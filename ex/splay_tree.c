/*
Copyright (c) 2012, Andrew Carter, Dietrich Lagenbach, Xanda Schofield
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

typedef char bool_t;
#define FALSE ((char) 0)
#define TRUE ((char)1)

typedef struct node* node;
#define LEFT 0
#define RIGHT 1
#define OTHER(n) (1 - (n))

struct node
{
    void* value_;
    node children_[2];
};

typedef int (*compare_t)(void * const,void * const);

typedef struct
{
    compare_t compare_;
    node root_;
} tree;

static node new_node(void * const value)
{
    node new = malloc(sizeof(struct node));
    new->value_ = value;
    new->children_[0] = new->children_[1] = NULL;
}

static node rotate(int const child, node const root, node out)
{
    node orphan;
    CHECK_ARGS(node);
    out = root->children_[child];
    orphan = out->children_[OTHER(child)];
    out->children_[OTHER(child)] = root;
    out->children_[OTHER(child)]->children_[child] = orphan;
    return out;
}


#define NO_ROTATE 2
#define NOT_FOUND 3
static int search_node(compare_t const compare, bool_t const insert,
                        node const this, void * const item, node out)
{
    int cmp, child, splay;
    CHECK_ARGS(node);
    if(this == NULL)
    {
        if(!insert)
        {
            out = NULL;
            return NOT_FOUND;
        }
        out = new_node(item);
        return NO_ROTATE;
    }
    out = this;
    cmp = compare(item, out->value_);
    if(!insert && cmp == 0)
        return NO_ROTATE;
    child = cmp < 0 ? LEFT : RIGHT;
    splay = search_node(compare, insert, out->children_[child], item, GET_RETURN(out->children_[child]));
    if(splay == NOT_FOUND)
        return NO_ROTATE;
    else if(splay == NO_ROTATE)
        return child;
    if(child == splay)
    {
        // zig-zig
        rotate(child, out, GET_RETURN(out));
        rotate(splay, out, GET_RETURN(out));
    }
    else
    {
        // zig-zag
        rotate(splay, out->children_[child], GET_RETURN(out->children_[child]));
        rotate(child, out, GET_RETURN(out));
    }
    return NO_ROTATE;
}
static int search_root(compare_t const compare, bool_t const insert,
                        node const root, void * const item, node out)
{
    int search;
    CHECK_ARGS(node);
    search = search_node(compare, insert, root, item, GET_RETURN(out));
    if(search == NOT_FOUND)
        return 0;
    if(search == NO_ROTATE)
        return compare(out->value_, item) == 0;
    rotate(search, out, GET_RETURN(out));
    return compare(out->value_, item) == 0;
}

static int find_min(void * const _, void * const __)
{
    return -1;
}

static int delete_node(compare_t const compare, node const root, void * const item, node out)
{
    int search;
    CHECK_ARGS(node);
    search = search_root(compare, FALSE, root, item, GET_RETURN(out));
    if(!search)
        return FALSE;
    search_root(find_min, FALSE, out->children_[RIGHT], NULL, GET_RETURN(out->children_[RIGHT]));
    out->children_[RIGHT]->children_[LEFT] = out->children_[LEFT];
    out = out->children_[RIGHT];
    return TRUE;
}

/* Tree stuff */
tree* new_tree(compare_t const compare, tree out)
{
    CHECK_ARGS(tree);
    out.compare_ = compare;
    out.root_ = NULL;
    return &out;
}

void insert(tree const this,void * const item, tree out)
{
    CHECK_ARGS(tree);
    out = this;
    search_root(out.compare_, TRUE, out.root_, item, GET_RETURN(out.root_));
}

int find(tree const this,void * const item, tree out)
{
    CHECK_ARGS(tree);
    out = this;
    return search_root(out.compare_, FALSE, out.root_, item, GET_RETURN(out.root_));
}

int delete(tree const this,void * const item, tree out)
{
    CHECK_ARGS(tree);
    out = this;
    return delete_node(out.compare_, out.root_, item, GET_RETURN(out.root_));
}

static void print_int_node(node const this)
{
    if(this == NULL)
    {
        printf("-");
        return;
    }
    printf("(");
    print_int_node(this->children_[LEFT]);
    printf(", %d, ", *((int*)this->value_));
    print_int_node(this->children_[RIGHT]);
    printf(")");
}

void print_tree(tree const this)
{
    print_int_node(this.root_);
    printf("\n");
}

int int_compare(void * const left,void * const right)
{
    return *((int*)left) - *((int*)right);
}

int main(void)
{
    int numbers[10] = {0,1,2,3,4,5,6,7,8,9};
    tree start = *new_tree(int_compare, NULL);
    print_tree(start);
    insert(start, numbers + 1, GET_RETURN(start));
    print_tree(start);
    insert(start, numbers + 3, GET_RETURN(start));
    print_tree(start);
    insert(start, numbers + 5, GET_RETURN(start));
    print_tree(start);
    insert(start, numbers + 7, GET_RETURN(start));
    print_tree(start);
    insert(start, numbers + 0, GET_RETURN(start));
    print_tree(start);
    insert(start, numbers + 4, GET_RETURN(start));
    print_tree(start);
    find(start, numbers + 7, GET_RETURN(start));
    print_tree(start);
    find(start, numbers + 2, GET_RETURN(start));
    print_tree(start);
    delete(start, numbers + 6, GET_RETURN(start));
    print_tree(start);
    delete(start, numbers + 0, GET_RETURN(start));
    print_tree(start);
    return 0;
}
