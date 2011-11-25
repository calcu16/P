#include "grammer.hpp"
#include "../Packrat/parser.hpp"
#include "../Packrat/symbol.hpp"
#include <string>

using namespace std;
using namespace packrat;

static Parser* singleton_pparser = NULL;

Parser* createParser()
{
    if(singleton_pparser)
    {
        Symbol digit = Symbol::createSet("0123456789");
        Symbol alpha = Symbol::createSet("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_");
        Symbol alphanum = digit | alpha;
        singleton_pparser = new Parser();
        singleton_pparser->addSymbol("c_comment", Symbol("/\\*(!(\\*/)|\\1)\\*/"));
        singleton_pparser->addSymbol("line_comment", Symbol("//(!\\n|1)\\n"));
        singleton_pparser->addSymbol("comment", Symbol("{c_comment}|{line_comment}"));
        singleton_pparser->addSymbol("space", Symbol("([ \t\r\n]|{comment})^0"));
    }
    return singleton_pparser;
}

#include <iostream>
#include "../Packrat/match.hpp"
int main(void)
{
	Parser* p = createParser();
	cout << p->parse("space", " /*weee*/ //yo\n") << endl;
	return 0;
}
