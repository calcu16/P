#include <string>
#include <iostream>
#include "Packrat/ast.hpp"
#include "Packrat/symbol.hpp"
#include "Packrat/parser.hpp"
#include "Packrat/pst.hpp"
#include "Packrat/build_tree.hpp"
#include "Wrapper/union.hpp"
#include "Printing/pprinter.hpp"

using namespace std;
using namespace packrat;
using namespace packrat::pst;
using namespace wrapper;


int main(void)
{
    AST temp = Parser::getPParser().parse("program","int foo() { c=a+b;} void bar() {}");
    Program func = buildTree<Program>(temp);
    // cout << temp << endl;
    // cout << "------------------" << endl;
    cout << func << endl;
    return 0;
}
