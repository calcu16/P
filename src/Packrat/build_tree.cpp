#include "build_tree.hpp"
#include <cassert>
#include <string>
#include <iostream>
using namespace std;
using namespace packrat;

template<>
BuildTree<bool>::BuildTree(const AST& tree)
{
    if(tree["value"].size() == 0)
        t_ = new bool(tree["value"].length() > 0);
    else if(*tree["value"]["type"] == "bool")
        t_ = new bool(*tree["value"]["value"] == "true");
    else
        t_ = NULL;
}

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

template<>
BuildTree<unsigned long long int>::BuildTree(const AST& tree)
{
    if(*tree["value"]["type"] == "int")
    {
        t_ = new unsigned long long int();
        std::istringstream(*tree["value"]["value"]) >> *t_;
    }
    else
        t_ = NULL;
}

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
