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

const string program =
"int foo(int a, int b)"
"{"
"   return a + b;"
"}"
""
"int main()"
"{"
"   return foo(a,b);"
"}"
;

int main(void)
{
    AST temp = Parser::getPParser().parse("program",program);
    cout << temp << endl;
    cout << "------------------" << endl;
    Program func = buildTree<Program>(temp);
    cout << func << endl;
    return 0;
}
