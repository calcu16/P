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
    
    template<typename T>
    struct BuildTree
    {
        T* t_;
        BuildTree(const AST&);
        ~BuildTree();
        void swap(BuildTree&);
        BuildTree<T>& operator=(BuildTree);
        
        T& operator*();
        const T& operator*() const;
    };

    template<typename L>
    struct BuildTree<std::list<L> >
    {
        typedef std::list<L> T;
        T* t_;
        BuildTree(const AST&);
        ~BuildTree();
        void swap(BuildTree&);
        BuildTree<T>& operator=(BuildTree);
        
        T& operator*();
        const T& operator*() const;
    };
    
    template<typename... TS>
    struct BuildTree<wrapper::Union<TS...> >
    {
        typedef wrapper::Union<TS...> T;
        static const size_t size = wrapper::type<0, TS...>::size;
        typedef typename table_t<size>::type name_t;
        
        T* t_;
        BuildTree(const AST&, name_t);
        
        ~BuildTree();
        void swap(BuildTree&);
        BuildTree<T>& operator=(BuildTree);
        
        T& operator*();
        const T& operator*() const;
    private:
        template<size_t I, typename... US>
        struct BuildUnion
        {
            
            BuildUnion(const AST&, name_t names, wrapper::Union<US...>&);
        };
        
        
        template<typename... US>
        struct BuildUnion<0, US...>
        {
            BuildUnion(const AST&, name_t, wrapper::Union<US...>&) {}
        };
    };
    
    template<typename T>
    T buildTree(const AST&);
    
    #include "build_tree.ipp"
}
#endif
