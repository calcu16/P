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
    template<size_t I>
    struct table_t {
        typedef char const * const type[I];
        // typedef std::string const * type;
    };
    template<typename S, typename R, typename SEP>
    struct fold_left_t {};
    
    template<typename T>
    struct BuildTree
    {
        typedef T return_t;
        T* t_;
        BuildTree(const T&);
        BuildTree(const AST&);
        template<size_t I>
        static T makeTree(const AST&, typename table_t<I>::type);
        
        ~BuildTree();
        void swap(BuildTree<T>&);
        BuildTree<T>& operator=(BuildTree<T>);
        
        T& operator*();
        const T& operator*() const;
    };

    template<typename L>
    struct BuildTree<std::list<L> >
    {
        typedef std::list<L> T;
        typedef T return_t;
        T* t_;
        BuildTree(const AST&);
        ~BuildTree();
        void swap(BuildTree<T>&);
        BuildTree<T>& operator=(BuildTree<T>);
        
        T& operator*();
        const T& operator*() const;
    };
    
    template<typename... TS>
    struct BuildTree<wrapper::Union<TS...> >
    {
        typedef wrapper::Union<TS...> T;
        typedef T return_t;
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
    
    template<typename... TS>
    struct BuildTree<std::tuple<TS...> >
    {
        typedef std::tuple<TS...> T;
        typedef T return_t;
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
    
    template<typename S, typename R, typename SEP>
    struct BuildTree<fold_left_t<S, R, SEP> >
    {
        typedef fold_left_t<S, R, SEP> T;
        typedef std::tuple<R, SEP, R> return_t;
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
    
    template<typename T>
    typename BuildTree<T>::return_t buildTree(const AST&);
    #include "build_tree.ipp"
}
#endif
