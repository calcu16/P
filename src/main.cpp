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
    ast["type"]  = AST(0,0,"Type");
    ast["value"] = AST(0,0,"Hello World!");
    
    
    packrat::pst::Type i = buildTree<packrat::pst::Type>(ast);
    
    test.set<1>("Hello World");
    cout << test.get<1>() << endl;
    cout << i.value_.get<0>() << endl;
    /*
    cout << Parser::getPParser().parse("program", "int main() { a = 0 return 0; }") << endl;
    */
    return 0;
}
