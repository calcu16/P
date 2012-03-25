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
using namespace wrapper;
int main(void)
{
    pst::Block b;
    Union<int, string> test;
    AST ast;
    
    packrat::pst::Expression i = buildTree<packrat::pst::Expression>(ast);
    
    test.set<1>("Hello World");
    cout << test.get<1>() << endl;
    /*
    cout << Parser::getPParser().parse("program", "int main() { a = 0 return 0; }") << endl;
    */
    return 0;
}
