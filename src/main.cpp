#include <string>
#include <iostream>
#include "Packrat/ast.hpp"
#include "Packrat/symbol.hpp"
#include "Packrat/parser.hpp"
using namespace std;
using namespace packrat;
int main(void)
{
    cout << "Hello World!" << endl;
    Symbol u("a[]bc-]!|d\\45!{a}{3}<args:({arg}:(,{arg})*)>|<what=hello>");
    cout << u << endl;
    return 0;
}
