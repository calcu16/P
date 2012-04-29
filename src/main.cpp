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
"void fw(const int **in, int length, int **out) {"
"   int i,j,k;"
"   out = in;"
"   for(k = 0; k < length; k = k + 1)"
"       for(i = 0; i < length; i = i + 1)"
"           for(j = 0; j < length; j = j + 1)"
"               if(out[i][j] > out[i][k] + out[k][j])"
"                   out[i][j] = out[i][k] + out[k][j];"
"}"
;

int main(void)
{
    AST temp = Parser::getPParser().parse("program",program);
    // cout << temp << endl;
    // cout << "------------------" << endl;
    Program func = buildTree<Program>(temp);
    cout << func << endl;
    return 0;
}
