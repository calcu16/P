#include "grammer.hpp"
#include "../Parser.hpp"
#include <string>

static Parser* singleton_pparser = NULL;

Parser* createParser()
{
    if(singleton_pparser)
    {
        Symbol digit = Symbol::createSet("0123456789");
        Symbol alpha = Symbol::createSet("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_");
        Symbol alphanum = digit / alpha;
        singleton_parser = new Parser();
        singleton_parser.addSymbol("c_comment", Symbol::createMatch("/*") + (-Symbol::createMatch("*/") + Symbol::next(1))^0 + Symbol::createMatch("*/"));
        singleton_parser.addSymbol("line_comment", Symbol::createMatch("//") + (-Symbol::createMatch("\n") + Symbol::next(1))^0 + Symbol::createMatch("\n"));
        singleton_parser.addSymbol("comment", Symbol::createLookup("c_comment") / Symbol::createLookup("line_comment"));
        singleton_parser.addSymbol("space", (Symbol::createSet(" \t\r\n") + Symbol::createLookup("comment"))^0);
        singleton_parser.addSymbol("ident", alpha + alphanum^0 + Symbol::createLookup("space"));
        singleton_parser.addSymbol("dint", -Symbol::createMatch("0") + digit^1 + Symbol::createLookup("space"));
        singleton_parser.addSymbol("oint", Symbol::createMatch("0") + digit^0 + Symbol::createLookup("space"));
        singleton_parser.addSymbol("hint", Symbol::createMatch("0x") + digit^0 + Symbol::createLookup("space"));
        singleton_parser.addSymbol("int", Symbol::createLookup("dint") / Symbol::createLookup("oint") / Symbol::createLookup("hint"));
        singleton_parser.addSymbol("atom", Symbol::createLookup("ident") / Symbol::createLookup("int"));
        singleton_parser.addSymbol("expr", Symbol::createLookup("atom"));
        singleton_parser.addSymbol("lval", Symbol::createLookup("ident"));
        singleton_parser.addSymbol("rval", Symbol::createLookup("atom"));
        singleton_parser.addSymbol("assign", Symbol::createLookup("lval") + (Symbol::createMatch("=") + Symbol::createLookup("space")) + Symbol::createLookup("rval"));
        singleton_parser.addSymbol("dec", Symbol::createLookup("ident") + Symbol::createLookup("assign") + ((Symbol::createMatch(",") + Symbol::createLookup("space")) + Symbol::createLookup("assign"))^0);
        singleton_parser.addSymbol("stmt", (Symbol::createLookup("dec") / Symbol::createLookup("assign")) + Symbol::createMatch(";") + Symbol::createLookup("space"));
        singleton_parser.addSymbol("ret_t", Symbol::createLookup("ident"));
        singleton_parser.addSymbol("func", Symbol::createLookup("ret_t") + Symbol::createLookup("ident") + Symbol::createMatch("(") + Symbol::createLookup("space") + Symbol::createLookup(")")
                                            + Symbol::createLookup("space") + Symbol::createMatch("{") + Symbol::createLookup("space") +
                                                Symbol::createLookup("stmts") ^ 0 + Symbol::createMatch("}") + Symbol::createLookup("space"));
        singleton_parser.addSymbol("program", Symbol::createLookup("func") ^ 0);
    }
    return singleton_parser
}
