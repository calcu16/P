#include "build_tree.hpp"
#include <string>
using namespace std;
using namespace packrat;

template<>
BuildTree<int>::BuildTree(const AST& tree)
{
    if(*tree["type"] == "int")
    {
        t_ = new int();
        std::istringstream(*tree["value"]) >> *t_;
    }
    else
        t_ = NULL;
}

template<>
BuildTree<string>::BuildTree(const AST& tree)
{
    if(*tree["type"] == "string")
    {
        // FIXME
        t_ = NULL;
    }
    else
        t_ = new string(*tree);
}
