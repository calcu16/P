#ifndef PACKRAT_BUILD_TREE_HPP
#define PACKRAT_BUILD_TREE_HPP
#include "ast.hpp"
#include "../Wrapper/union.hpp"

namespace packrat
{
    template<const char*...>
    class strings;
    
    template<typename T>
    class BuildTree
    {
        T* t_;
        BuildTree(const AST&);
        T operator*();
    };


    template<typename T>
    T buildTree(const AST&);
}
#include "build_tree.ipp"
#endif
