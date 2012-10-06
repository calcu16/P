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
#include <iostream>
/* We construct returned trees from ASTs */
template<typename T>
typename BuildTree<T>::return_t buildTree(const AST& tree)
{
    assert(tree);
    BuildTree<T> build(tree);
    return *build;
}

/* Generic destructor for standard objects */
template<typename T>
BuildTree<T>::~BuildTree()
{
    delete t_;
}

/* Destructor for trees of lists */
template<typename L>
BuildTree<std::list<L> >::~BuildTree()
{
    delete t_;
}

/* Destructor for trees of unions */
template<typename... US>
BuildTree<wrapper::Union<US...> >::~BuildTree()
{
    delete t_;
}

/* Destructor for trees of tuples */
template<typename... TS>
BuildTree<std::tuple<TS...> >::~BuildTree()
{
    delete t_;
}

/* Destructor for trees assembled with fold_left */
template<typename S, typename R, typename SEP>
BuildTree<fold_left_t<S, R, SEP> >::~BuildTree()
{
    delete r_;
}

/* Destructor for trees assembled with postfix */
template<typename S, typename R, typename SEP>
BuildTree<postfix_t<S, R, SEP> >::~BuildTree()
{
    delete r_;
}

/* Standard tree swap */
template<typename T>
void BuildTree<T>::swap(BuildTree<T>& other)
{
    std::swap(t_, other.t_);
}

/* List tree swap */
template<typename L>
void BuildTree<std::list<L> >::swap(BuildTree<T>& other)
{
    std::swap(t_, other.t_);
}

/* Union tree swap */
template<typename... US>
void
BuildTree<wrapper::Union<US...> >::swap(BuildTree<T>& other)
{
    std::swap(t_, other.t_);
}

/* Tuple tree swap */
template<typename... TS>
void
BuildTree<std::tuple<TS...> >::swap(BuildTree<T>& other)
{
    std::swap(t_, other.t_);
}

/* Fold_left tree swap */
template<typename S, typename R, typename SEP>
void
BuildTree<fold_left_t<S, R, SEP> >::swap(BuildTree<T>& other)
{
    std::swap(r_, other.r_);
}

/* postfix tree swap */
template<typename S, typename R, typename SEP>
void
BuildTree<postfix_t<S, R, SEP> >::swap(BuildTree<T>& other)
{
    std::swap(r_, other.r_);
}

/* Standard tree assignment op */
template<typename T>
BuildTree<T>& BuildTree<T>::operator=(BuildTree<T> rhs)
{
    swap(rhs);
}

/* List tree assignment op */
template<typename L>
BuildTree<std::list<L> >&
BuildTree<std::list<L> >::operator=(BuildTree<T> rhs)
{
    swap(rhs);
}

/* Union tree assignment op */
template<typename... US>
BuildTree<wrapper::Union<US...> >&
BuildTree<wrapper::Union<US...> >::operator=(BuildTree<T> rhs)
{
    swap(rhs);
}

/* Tuple tree assignment op */
template<typename... TS>
BuildTree<std::tuple<TS...> >&
    BuildTree<std::tuple<TS...> >::operator=(BuildTree<T> rhs)
{
    swap(rhs);
}

/* Fold_left tree assignment op */
template<typename S, typename R, typename SEP>
BuildTree<fold_left_t<S, R, SEP> >&
BuildTree<fold_left_t<S, R, SEP> >::operator=(BuildTree<T>)
{
}

/* postfix tree assignment op */
template<typename S, typename R, typename SEP>
BuildTree<postfix_t<S, R, SEP> >&
BuildTree<postfix_t<S, R, SEP> >::operator=(BuildTree<T>)
{
}


/* Generic tree dereference */
template<typename T>
T& BuildTree<T>::operator*()
{
    return *t_;
}

/* List tree dereference */
template<typename L>
std::list<L>& BuildTree<std::list<L> >::operator*()
{
    return *t_;
}

/* Union tree dereference */
template<typename... US>
wrapper::Union<US...>&
BuildTree<wrapper::Union<US...> >::operator*()
{
    return *t_;
}

/* Tuple tree dereference */
template<typename... TS>
std::tuple<TS...>& BuildTree<std::tuple<TS...> >::operator*()
{
    return *t_;
}

/* Fold_left tree dereference */
template<typename S, typename R, typename SEP>
std::tuple<R, SEP, R>& BuildTree<fold_left_t<S, R, SEP> >::operator*()
{
    return *r_;
}

/* postfix tree dereference */
template<typename S, typename R, typename SEP>
std::tuple<R, SEP>& BuildTree<postfix_t<S, R, SEP> >::operator*()
{
    return *r_;
}

/* Generic tree const dereference */
template<typename T>
const T& BuildTree<T>::operator*() const
{
    return *t_;
}

/* List tree const dereference */
template<typename L>
const std::list<L>& BuildTree<std::list<L> >::operator*() const
{
    return *t_;
}

/* Union tree const dereference */
template<typename... US>
const wrapper::Union<US...>&
    BuildTree<wrapper::Union<US...> >::operator*() const
{
    return *t_;
}

/* Tuple tree const dereference */
template<typename... TS>
const std::tuple<TS...>& BuildTree<std::tuple<TS...> >::operator*() const
{
    return *t_;
}

/* Fold_left tree const dereference */
template<typename S, typename R, typename SEP>
const std::tuple<R, SEP, R>& \
BuildTree<fold_left_t<S, R, SEP> >::operator*() const
{
    return *r_;
}

/* postfix tree const dereference */
template<typename S, typename R, typename SEP>
const std::tuple<R, SEP>&
BuildTree<postfix_t<S, R, SEP> >::operator*() const
{
    return *r_;
}



/* Constructor given an AST */
template<typename T>
BuildTree<T>::BuildTree(const AST& tree)
    : t_(new T()) // initialize tree itself
{
    // Leverages makeTree to construct the tree from the AST
    t_->value_ = BuildTree<typename T::type>::template
            makeTree<T::names_l>(tree["value"], T::names);
}

// Templates instantiating BuildTree for int and string
template<>
BuildTree<bool>::BuildTree(const AST& tree);

template<>
BuildTree<int>::BuildTree(const AST& tree);

template<>
BuildTree<unsigned long long int>::BuildTree(const AST& tree);

template<>
BuildTree<std::string>::BuildTree(const AST& tree);

/* List tree constructor */
template<typename L>
BuildTree<std::list<L> >::BuildTree(const AST& tree)
{
    assert(tree["value"].size() == 0);
    t_ = new std::list<L>(); // initialize the list within the tree
    // Build each of the trees in the AST's list structure and add it
    // to the new tree
    for(AST::const_iterator i = tree["value"].begin();
            i != tree["value"].end(); ++i)
        t_->push_back(buildTree<L>(*i));
}

/* Union tree constructor */
template<typename... US>
BuildTree<wrapper::Union<US...> >::
    BuildTree(const AST& tree, name_t names)
{
    // std::cout << "Building a Union" << std::endl;
    // Uses helper BuildUnion to store the constructed tree in t_ 
    t_ = new wrapper::Union<US...>();
    BuildUnion<size, US...>(tree, names, *t_);
    
    if((int)(*t_) == -1)
    {
        std::cerr << tree << std::endl;
        std::cerr << "Does not match anything in " << std::endl;
        for(size_t i = 0; i < size; ++i)
            std::cerr << "\t" << names[i] << std::endl;
        assert(false);
    }
    assert((int)(*t_) != -1);
}

/* BuildUnion: Helper for union tree constructor */
template<typename... TS>
template<size_t I, typename... US>
BuildTree<wrapper::Union<TS...> >::BuildUnion<I, US...>::BuildUnion
    (const AST& tree, name_t names, T& result)
{
    typedef typename wrapper::type<I-1, US...>::value type;
    /*
    std::cout << "Looking at " << I-1 << std::endl;
    std::cout << "Comparing '" << names[I-1] << "' with '"
              << *tree["type"] << "'." << std::endl;
    */
    if(*tree["type"] == names[I-1])
        result.template set<I-1>(buildTree<type>(tree));
    else
        BuildUnion<I-1,US...>(tree, names, result);
}

template<typename... TS>
template<typename... US>
BuildTree<wrapper::Union<TS...> >::BuildUnion<0, US...>::BuildUnion
    (const AST&, name_t, T&)
{
}

/* Tuple tree constructor */
template<typename... TS>
BuildTree<std::tuple<TS...> >::BuildTree(const AST& tree, name_t names)
{
    // Uses BuildTuple helper to create the tree
    t_ = new std::tuple<TS...>();
    BuildTuple<size, TS...>(tree, names, *t_);
}

/* BuildTuple: helper for making trees for tuples */
template<typename... TS>
template<size_t I, typename... US>
BuildTree<std::tuple<TS...> >::BuildTuple<I, US...>::BuildTuple
    (const AST& tree, name_t names, T& result)
{
    typedef typename wrapper::type<I-1, US...>::value type;
    std::get<I-1>(result) = buildTree<type>(tree[names[I-1]]);
    BuildTuple<I-1,US...>(tree, names, result);
}

template<typename... TS>
template<typename... US>
BuildTree<std::tuple<TS...> >::BuildTuple<0, US...>::BuildTuple
    (const AST&, name_t, T&)
{
}
#include <iostream>
/* fold_left tree constructor */
template<typename S, typename R, typename SEP>
BuildTree<fold_left_t<S, R, SEP> >::BuildTree(const AST& tree, name_t names)
    : r_(NULL)
{
    // Sets up iterator for loop to construct fold_left structure
    AST::const_iterator i = tree.begin();
    std::string value = names[0], sep = names[1];
    // Make the first element in the innermost level
    R left = buildTree<R>((*i)[value]);

    // Make a tuple containing the previous tuple and the next level
    for(++i; i != tree.end(); ++i)
    {
        r_ = new std::tuple<R,SEP,R>
            (left,
             buildTree<SEP>((*i)[sep]),
             buildTree<R>((*i)[value]));
        S holder;
        holder.value_ = *r_;
        left = (R)holder;
    }
}

/* postfix tree constructor */
template<typename S, typename R, typename OP>
BuildTree<postfix_t<S, R, OP> >::BuildTree(const AST& tree, name_t names)
    : r_(NULL)
{
    // Sets up iterator for loop to construct fold_left structure
    std::string value = names[0], op = names[1];
    // Make the first element in the innermost level
    R left = buildTree<R>(tree[value]);

    // Make a tuple containing the previous tuple and the next level
    for(AST::const_iterator i = tree[op].begin();
            i != tree[op].end(); ++i)
    {
        r_ = new std::tuple<R,OP>(left, buildTree<OP>(*i));
        S holder;
        holder.value_ = *r_;
        left = (R)holder;
    }
}

// Generic makeTree
template<typename T>
template<size_t I>
T BuildTree<T>::makeTree(const AST& tree, typename table_t<I>::type names)
{
    BuildTree<T> build(tree, names);
    return *build;
}

// makeTree for unions
template<typename... US>
template<size_t I>
wrapper::Union<US...> BuildTree<wrapper::Union<US...> >::
    makeTree(const AST& tree, typename table_t<I>::type names)
{
    BuildTree<T> build(tree, names);
    return *build;
}

// makeTree for tuples
template<typename... TS>
template<size_t I>
std::tuple<TS...> BuildTree<std::tuple<TS...> >::
    makeTree(const AST& tree, typename table_t<I>::type names)
{
    BuildTree<T> build(tree, names);
    return *build;
}

// makeTree for fold_left structure
template<typename S, typename R, typename SEP>
template<size_t I>
std::tuple<R, SEP, R> BuildTree<fold_left_t<S, R, SEP> >::
    makeTree(const AST& tree, typename table_t<I>::type names)
{
    BuildTree<T> build(tree, names);
    return *build;
}

// makeTree for postfix structure
template<typename S, typename R, typename SEP>
template<size_t I>
std::tuple<R, SEP> BuildTree<postfix_t<S, R, SEP> >::
    makeTree(const AST& tree, typename table_t<I>::type names)
{
    BuildTree<T> build(tree, names);
    return *build;
}

#include <iostream>
// makeTree for int trees
template<>
template<size_t I>
int BuildTree<int>::makeTree(const AST& tree, typename table_t<I>::type names)
{
    size_t i;
    for(i = 0; i < I; ++i)
        if(*tree == names[i])
            break;
    return i;
}
