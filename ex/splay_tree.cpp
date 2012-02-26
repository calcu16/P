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
typedef Pointer<struct_node> node;
/// #define LEFT 0
#define LEFT 0
/// #define RIGHT 1
#define RIGHT 1
/// #define OTHER(n) (1 - (n))
#define OTHER(n) (1 - (n))

/// struct node
struct struct_node
{
    /// void* value_;
    Pointer<int> value_;
    /// node children_[2]
    node children_[2];
};


/* Specializing for now, needs to be more type aware in the future */
/// typedef int (*compare_t)(void * const,void * const);
typedef struct { int value; } (*compare_t)(Pointer<int> *, Pointer<int> *)

/// typedef struct
typedef struct_tree tree;
struct struct_tree
{
    /// compare_t compare
    compare_t compare_;
    /// node root_
    node root_;
};

/// static node new_node(void * const value)
static struct { node value; } new_node(Pointer<int> value)
{
    struct { node value} __return__;
    /// node new = malloc(sizeof(struct node));
    node _new(struct_node());
    /// new->value_ = value;
    _new->value_ = value;
    /// new->children_[0] = new->children_[1] = NULL;
    _new->children_[0] = new_->children_[1] = NULL;
    /// return new;
    __return__.value = _new
    return _new;
}

/// static node rotate(int const child, node const root, node out)
static struct { node value; node out; } rotate(int child, node root)
{
    struct { node value; node out; } __return__;
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
///                         node const this, void * const item, node out
static struct { int value; node out; }
            search_node(compare_t const compare,bool_t const insert,
                        node const _this, void * const item)
{
    struct { int value; node out; } __return__;
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
        __return__.out = new_node(item);
        /// return NO_ROTATE;
        __return__.value = NO_ROTATE;
        return __return__;
    }
    /// out = this;
    __return__.out = _this;
    /// cmp = compare(item, out->value_);
    {
        struct { int value; } __result__ = compare(item, __return__.out->value_);
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
        struct { int value; node out; } __result__ = search_node(compare, insert, out->children_[child], item);
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
            struct { node value; node out; } __result__ = rotate(child, __return__.out);
            __return__.out = __result__.out;
        }
        /// rotate(splay, out, out);
        {
            struct { node value; node out; } __result__ = rotate(splay, __return__.out);
            __return__.out = __result__.out;
        }
    }
    else
    {
        // zig-zag
        /// rotate(child, out->children_[child], out->children_[child]);
        {
            struct { node value; node out; } __result__ = rotate(splay, __return__.out->children_[child]);
            __return__.out->children_[child] = __result__.out;
        }
        /// rotate(child, out, out);
        {
            struct { node value; node out; } __result__ = rotate(child, __return__.out);
            __return__.out = __result__.out;
        }
    }
    /// return NO_ROTATE;
    __return__.value = NO_ROTATE;
    return __return__;
}
