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
    AST temp = Parser::getPParser().parse("expression","name");
    Expression exp = buildTree<Expression>(temp);
    cout << temp << endl;
    cout << (int)exp.value_ << endl;
    cout << exp.value_.get<0>() << endl;
    return 0;
}
