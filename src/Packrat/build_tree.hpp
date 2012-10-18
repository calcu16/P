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
#ifndef PACKRAT_BUILD_TREE_HPP
#define PACKRAT_BUILD_TREE_HPP
#include "ast.hpp"
#include "../Wrapper/union.hpp"
#include "../Wrapper/type.hpp"
#include <tuple>
#include <list>
#include <vector>
#include <sstream>
#include <string>

namespace packrat
{
    /*
     * "Templated typedef" to define strings as table_ts of size I
     */
    template<typename T, size_t I>
    struct array_t {
        typedef T const type[I];
    };
    template<size_t I>
    struct table_t {
        typedef typename array_t<const char*,I>::type type;
    };
    
    
    
    /*
     * A trait indicating the conversion of a list to a fold-left tree
     */
    template<typename S, typename R, typename SEP>
    struct fold_left_t {};
    
    template<typename S, typename R, typename OP>
    struct postfix_t {};
    
    /*
     * BuildTrees are wrappers for things built from abstract syntax 
     *   trees. These are intended for the creation of new syntax trees.
     */
    template<typename T>
    struct BuildTree
    {
        typedef typename table_t<1>::type name_t;
        typedef T return_t;    // typedef to make return types for functions
        T* t_;                 // stored value
        BuildTree(const AST&); // function to build a tree from an abstract 
                               //   syntax tree
        BuildTree(const AST&, name_t);
        /* static function to make a tree based upon which item specified by
         * the AST among the values enumerated in the table_t.
         */
        template<size_t I>     
        static T makeTree(const AST&, typename table_t<I>::type);
        
        ~BuildTree();                           // destructor
        void swap(BuildTree<T>&);               // swap function
        BuildTree<T>& operator=(BuildTree<T>);  // assignment operator
        
        T& operator*();                 // accessor
        const T& operator*() const;     // const accessor
    };

    /* This BuildTree struct works on lists of objects */
    template<typename L>
    struct BuildTree<std::list<L> >
    {
        typedef std::list<L> T; // convenience typedef for consistency
        typedef T return_t;     // typedef for return type
        T* t_;                  // stored value
        BuildTree(const AST&);  // function to build the tree from an 
                                //   abstract syntax tree
                                
        ~BuildTree();                           // destructor
        void swap(BuildTree<T>&);               // swap function
        BuildTree<T>& operator=(BuildTree<T>);  // assignment operator
        
        T& operator*();               // accessor
        const T& operator*() const;   // const accessor
    };
    
    /* This BuildTree struct works on unions */
    template<typename... TS>
    struct BuildTree<wrapper::Union<TS...> >
    {
        typedef wrapper::Union<TS...> T; 
        typedef T return_t;              
        // describes the number of elements of the union
        static const size_t size = wrapper::type<0, TS...>::size;
        typedef typename table_t<size>::type name_t;
        
        T* t_;
        BuildTree(const AST&, name_t);
        template<size_t I>
        static T makeTree(const AST&, typename table_t<I>::type);
        
        ~BuildTree();
        void swap(BuildTree<T>&);
        BuildTree<T>& operator=(BuildTree<T>);
        
        T& operator*();
        const T& operator*() const;
    
    // helpers for template specializations
    private:
        template<size_t I, typename... US>
        struct BuildUnion
        {
            BuildUnion(const AST&, name_t names, T&);
        };
        
        
        template<typename... US>
        struct BuildUnion<0, US...>
        {
            BuildUnion(const AST&, name_t, T&);
        };
    };
    
    /* This BuildTree struct works on tuples */
    template<typename... TS>
    struct BuildTree<std::tuple<TS...> >
    {
        typedef std::tuple<TS...> T;
        typedef T return_t;
        // describes the number of elements in the tuple
        static const size_t size = wrapper::type<0, TS...>::size;
        typedef typename table_t<size>::type name_t;
        
        T* t_;
        BuildTree(const AST&, name_t);
        template<size_t I>
        static T makeTree(const AST&, typename table_t<I>::type);
        
        ~BuildTree();
        void swap(BuildTree<T>&);
        BuildTree<T>& operator=(BuildTree<T>);
        
        T& operator*();
        const T& operator*() const;
        
    // helpers for template specialization
    private:
        template<size_t I, typename... US>
        struct BuildTuple
        {
            BuildTuple(const AST&, name_t names, T&);
        };
        
        
        template<typename... US>
        struct BuildTuple<0, US...>
        {
            BuildTuple(const AST&, name_t, T&);
        };
    };
    
    /* Specifictions for a left folding BuildTree */
    template<typename S, typename R, typename SEP>
    struct BuildTree<fold_left_t<S, R, SEP> >
    {
        typedef fold_left_t<S, R, SEP> T;
        typedef std::tuple<R, SEP, R> return_t;
        // size of the table for names
        static const size_t size = 2; 
        typedef typename table_t<size>::type name_t;
        
        return_t* r_;
        template<size_t I>
        static return_t makeTree(const AST&, typename table_t<I>::type);
        
        BuildTree(const AST&, name_t);
        
        ~BuildTree();
        void swap(BuildTree<T>&);
        BuildTree<T>& operator=(BuildTree<T>);
        
        return_t& operator*();
        const return_t& operator*() const;
    };
    
    /* Specifictions for a postfix operator */
    template<typename S, typename R, typename OP>
    struct BuildTree<postfix_t<S, R, OP> >
    {
        typedef postfix_t<S, R, OP> T;
        typedef std::tuple<R, OP> return_t;
        static const size_t size = 2; // requires to names for postfix operator
        typedef typename table_t<size>::type name_t;
        
        return_t* r_;
        template<size_t I>
        static return_t makeTree(const AST&, typename table_t<I>::type);
        
        BuildTree(const AST&, name_t);
        
        ~BuildTree();
        void swap(BuildTree<T>&);
        BuildTree<T>& operator=(BuildTree<T>);
        
        return_t& operator*();
        const return_t& operator*() const;
    };
    
    template<typename T>
    typename BuildTree<T>::return_t buildTree(const AST&);
    #include "build_tree.ipp"
}
#endif
