#include "fat_pointer.hpp"
/// #include <stdlib.h>
#include <stdlib.h>
/// #include <stdio.h>
#include <stdio.h>

/// typedef char bool_t
typedef char bool_t;
/// #define FALSE ((char) 0)
#define FALSE ((char) 0)
/// #define TRUE ((char)1)
#define TRUE ((char)1)

/// typedef struct node* node
struct struct_node;
typedef FatPointer<struct_node> node;
/// #define LEFT 0
#define LEFT 0
/// #define RIGHT 1
#define RIGHT 1
/// #define OTHER(n) (1 - (n))
#define OTHER(n) (1 - (n))

/// struct node
struct struct_node
{
    /// int* value_;
    FatPointer<int> value_;
    /// node children_[2]
    node children_[2];
};


/* Specializing for now, needs to be more type aware in the future */
/// typedef int (*compare_t)(int * const,int * const);
struct compare_t_return { int value; };
typedef compare_t_return (*compare_t)(FatPointer<int>, FatPointer<int>);

/// typedef struct
struct struct_tree
{
    /// compare_t compare
    compare_t compare_;
    /// node root_
    node root_;
};
typedef struct_tree tree;

/// static node new_node(int * const value)
struct new_node_return { node value; };
static new_node_return new_node(FatPointer<int> value)
{
    new_node_return __return__;
    /// node new = malloc(sizeof(struct node));
    node _new = node(struct_node());
    /// new->value_ = value;
    _new->value_ = value;
    /// new->children_[0] = new->children_[1] = NULL;
    _new->children_[0] = NULL;
    _new->children_[1] = NULL;
    /// return new;
    __return__.value = _new;
    return __return__;
}

/// static node rotate(int const child, node const root, node out)
struct rotate_return { node value; node out; };
static rotate_return rotate(int child, node root)
{
    rotate_return __return__;
    /// node orphan;
    node orphan;
    /// out = root->children_[child];
    __return__.out = root->children_[child];
    /// orphan = out->children_[OTHER(child)];
    orphan = __return__.out->children_[OTHER(child)];
    /// out->children_[OTHER(child)] = root;
    __return__.out->children_[OTHER(child)] = root;
    /// out->children_[OTHER(child)]->children_[child] = orphan;
    __return__.out->children_[OTHER(child)]->children_[child] = orphan;
    /// return out;   
    __return__.value = __return__.out;
    return __return__;
}

/// #define NO_ROTATE 2
#define NO_ROTATE 2
/// #define NOT_FOUND 3
#define NOT_FOUND 3
/// static int search_node(compare_t const compare, bool_t const insert,
///                         node const this, int * const item, node out
struct search_node_return { int value; node out; };
static search_node_return search_node(compare_t compare,bool_t insert,
                        node _this, FatPointer<int> item)
{
    struct search_node_return __return__;
    ///  int cmp, child, splay;
    int cmp, child, splay;
    /// if(this == NULL)
    if(_this == NULL)
    {
        /// if(!insert)
        if(!insert)
        {
            /// out = NULL;
            __return__.out = node();
            /// return NOT_FOUND;
            __return__.value = NOT_FOUND;
            return __return__;
        }
        /// out = new_node(item);
        {
            new_node_return __result__ = new_node(item);
            __return__.out = __result__.value;
        }
        /// return NO_ROTATE;
        __return__.value = NO_ROTATE;
        return __return__;
    }
    /// out = this;
    __return__.out = _this;
    /// cmp = compare(item, out->value_);
    {
        compare_t_return __result__ = compare(item, __return__.out->value_);
        cmp = __result__.value;
    }
    /// if(!insert && cmp == 0)
    if(!insert && cmp == 0)
    {
        /// return NO_ROTATE;
        __return__.value = NO_ROTATE;
        return __return__;
    }
    child = cmp < 0 ? LEFT : RIGHT;
    /// splay = search_node(compare, insert, out->children_[child], item, out->children_[child]);
    {
        search_node_return __result__ = search_node(compare, insert, __return__.out->children_[child], item);
        __return__.out->children_[child] = __result__.out;
        splay = __result__.value;
    }
    /// if(splay == NOT_FOUND)
    if(splay == NOT_FOUND)
    {
        /// return NO_ROTATE;
        __return__.value = NO_ROTATE;
        return __return__;
    }
    /// else if(splay == NO_ROTATE)
    else if(splay == NO_ROTATE)
    {
        /// return child;
        __return__.value = child;
        return __return__;
    }
    /// if(child == splay)
    if(child == splay)
    {
        // zig-zig
        /// rotate(child, out, out);
        {
            rotate_return __result__ = rotate(child, __return__.out);
            __return__.out = __result__.out;
        }
        /// rotate(splay, out, out);
        {
            rotate_return __result__ = rotate(splay, __return__.out);
            __return__.out = __result__.out;
        }
    }
    else
    {
        // zig-zag
        /// rotate(child, out->children_[child], out->children_[child]);
        {
            rotate_return __result__ = rotate(splay, __return__.out->children_[child]);
            __return__.out->children_[child] = __result__.out;
        }
        /// rotate(child, out, out);
        {
            rotate_return __result__ = rotate(child, __return__.out);
            __return__.out = __result__.out;
        }
    }
    /// return NO_ROTATE;
    __return__.value = NO_ROTATE;
    return __return__;
}

/// static int search_root(compare_t const compare, bool_t const insert,
///                        node const root, int * const item, node out)
struct search_root_return { int value; node out; };
static search_root_return search_root(compare_t compare, bool_t insert,
                        node root, FatPointer<int> item)
{
    search_root_return __return__;
    /// int search;
    int search;
    /// search = search_node(compare, insert, root, item, GET_RETURN(out));
    {
        search_node_return __result__ = search_node(compare, insert, root, item);
        __return__.out = __result__.out;
        search = __result__.value;
    }
    /// if(search = NOT_FOUND)
    if(search == NOT_FOUND)
    {
        /// return FALSE;
        __return__.value = FALSE;
        return __return__;
    }
    /// if(search != NO_ROTATE)
    if(search != NO_ROTATE)
    {
        /// rotate(search, out, out);
        {
            rotate_return __result__ = rotate(search, __return__.out);
            __return__.out = __result__.out;
        }
    }
    
    /// return compare(out->value_, item) == 0;
    {
        compare_t_return __result__ = compare(__return__.out->value_, item);
        __return__.value == 0;
        
    }
    return __return__;
    
}

/// static int find_min(int * const _, int * const __)
static compare_t_return find_min(FatPointer<int> _, FatPointer<int> __)
{
    compare_t_return __return__;
    ///  return -1;
    __return__.value = -1;
    return __return__;
}

/// static int delete_node(compare_t const compare, node const root, int * const item, node out)
struct delete_node_return { int value; node out; };
static delete_node_return delete_node(compare_t compare, node root, FatPointer<int> item)
{
    delete_node_return __return__;
    /// int search;
    int search = true;
    /// search = search_root(compare, FALSE, root, item, out);
    {
        search_root_return __result__ = search_root(compare, FALSE, root, item);
        __return__.out = __result__.out;
        search = __result__.value;
    }
    /// if(!search)
    if(!search)
    {
        /// return FALSE;
        __return__.value = FALSE;
        return __return__;
    }
    /// search_root(find_min, FALSE, out->children_[RIGHT], NULL, out->children_[RIGHT]);
    {
        search_root_return __result__ = search_root(find_min, FALSE, __result__.out->children_[RIGHT], FatPointer<int>());
        __return__.out->children_[RIGHT] = __result__.out;
    }
    /// out->children_[RIGHT]->children_[LEFT] = out->children_[LEFT];
    __return__.out->children_[RIGHT]->children_[LEFT] = __return__.out->children_[LEFT];
    /// out = out->children_[RIGHT];
    __return__.out = __return__.out->children_[RIGHT];
    /// return TRUE;
    __return__.value = TRUE;
    return __return__;
}

/* Tree stuff */
/// tree* new_tree(compare_t const compare, tree out)
struct new_tree_return { FatPointer<tree> value; tree out; };
new_tree_return new_tree(compare_t compare)
{
    new_tree_return __return__;
    /// out.compare_ = compare;
    __return__.out.compare_ = compare;
    /// out.root_ = NULL;
    __return__.out.root_ = node();
    /// return &out;
    __return__.value = FatPointer<tree>(__return__.out);
    return __return__;
}

/// void insert(tree const this,int * const item, tree out)
struct insert_return { tree out; };
insert_return insert(tree _this, FatPointer<int> item)
{
    insert_return __return__;
    /// out = this;
    __return__.out = _this;
    /// search_root(out.compare_, TRUE, out.root_, item, out.root_);
    {
        search_root_return __result__ = search_root(__return__.out.compare_, TRUE, __return__.out.root_, item);
        __return__.out.root_ = __result__.out;
    }
    ///
    return __return__;
}

/// int find(tree const this,int * const item, tree out)
struct find_return { int value; tree out; };
find_return find(tree _this,FatPointer<int> item)
{
    find_return __return__;
    /// out = this;
    __return__.out = _this;
    /// return search_root(out.compare_, FALSE, out.root_, item, out.root_);
    {
        search_root_return __result__ = search_root(__return__.out.compare_, FALSE, __return__.out.root_, item);
        __return__.out.root_ = __result__.out;
    }
    return __return__;
}

/// int delete(tree const this,int * const item, tree out)
struct delete_return { int value; tree out; };
delete_return _delete(tree _this, FatPointer<int> item)
{
    delete_return __return__;
    /// out = this;
    __return__.out = _this;
    /// return delete_node(out.compare_, out.root_, item, out.root_);
    {
        delete_node_return __result__ = delete_node(__return__.out.compare_, __return__.out.root_, item);
        __return__.out.root_ = __result__.out;
        __return__.value = __result__.value;
    }
    return __return__;
}

/// static void print_int_node(node const this)
static void print_int_node(node _this)
{
    /// if(this == NULL)
    if(_this == NULL)
    {
        /// printf("-");
        printf("-");
        /// return;
        return;
    }
    /// printf("(");
    printf("(");
    /// print_int_node(this->children_[LEFT]);
    print_int_node(_this->children_[LEFT]);
    /// printf(", %d, ", *this->value_);
    printf(", %d, ", *_this->value_);
    /// print_int_node(this->children_[RIGHT]);
    print_int_node(_this->children_[RIGHT]);
    /// printf(")");
    printf(")");
}

/// void print_tree(tree const this)
void print_tree(tree _this)
{
    /// print_int_node(this.root_);
    print_int_node(_this.root_);
    /// printf("\n");
    printf("\n");
}

/// int int_compare(int * const left,int * const right)
compare_t_return int_compare(FatPointer<int> left,FatPointer<int> right)
{
    compare_t_return __return__;
    // return *left - *right;
    __return__.value = *left - *right;
    return __return__;
}

int main(void)
{
    tree start = *new_tree(int_compare).value;
    print_tree(start);
    start = insert(start, FatPointer<int>(1)).out;
    print_tree(start);
    start = insert(start, FatPointer<int>(3)).out;
    print_tree(start);
    start = insert(start, FatPointer<int>(5)).out;
    print_tree(start);
    start = insert(start, FatPointer<int>(7)).out;
    print_tree(start);
    start = insert(start, FatPointer<int>(0)).out;
    print_tree(start);
    start = insert(start, FatPointer<int>(4)).out;
    print_tree(start);
    insert(start, FatPointer<int>(2));
    print_tree(start);
    start = find(start, FatPointer<int>(7)).out;
    print_tree(start);
    start = find(start, FatPointer<int>(2)).out;
    print_tree(start);
}