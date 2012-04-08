#include "build_tree.hpp"
#include <cassert>
#include <string>
using namespace std;
using namespace packrat;

template<>
BuildTree<int>::BuildTree(const AST& tree)
{
    if(*tree["value"]["type"] == "int")
    {
        t_ = new int();
        std::istringstream(*tree["value"]["value"]) >> *t_;
    }
    else
        t_ = NULL;
}

#include <iostream>
template<>
BuildTree<string>::BuildTree(const AST& tree)
{
    if(*tree["value"]["type"] == "StringConst")
    {
        // FIXME
        t_ = NULL;
    }
    else if(*tree["value"]["type"] == "Ident")
    {
        t_ = new string(*tree["value"]["value"]);
    }
    else
    {
        t_ = new string(*tree["value"]);
    }
}
