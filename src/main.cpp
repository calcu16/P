#include <string>
#include <iostream>
#include "Packrat/ast.hpp"
#include "Packrat/symbol.hpp"
#include "Packrat/parser.hpp"
#include "Packrat/pst.hpp"
#include "Packrat/build_tree.hpp"
#include "Wrapper/union.hpp"

using namespace std;
using namespace packrat;
using namespace packrat::pst;
using namespace wrapper;


int main(void)
{
    AST temp = Parser::getPParser().parse("function","void foo() { c=a+b; } int bar() {return var;}");
    Program func = buildTree<Program>(temp);
    return 0;
}
