#include "parser.hpp"
#include "match.hpp"
#include "symbol.hpp"
#include <string>
#include <iostream>

using namespace std;
using namespace packrat;

int main(void)
{
	Parser p;
	p.addSymbol("int", Symbol("!0<value:[0-9]+_>"));
	p.addSymbol("temp", Symbol("<test:a*><other=b>"));
	p.addSymbol("add", Symbol("<lrec:{int}\\+>*<last:{int}>"));
	Match m(p.parse("add","123+456"));
	cout << m << endl;
	return 0;
}

