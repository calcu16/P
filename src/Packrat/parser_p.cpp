#include "parser.hpp"
using namespace std;
using namespace packrat;
static Parser* PParser = NULL;
const Parser& Parser::getPParser()
{
    if(PParser == NULL)
        PParser = new Parser(
            /* Lexcical Analysis */
            /** Seperators **/
            "COMMENT_S",    "/\\*",
            "COMMENT_E",    "\\*/",
            "COMMENT",      "{COMMENT_S}(!{COMMENT_E}.)*_{COMMENT_E}",
            "SPACE",        "[ \t\r\n\v]",
            "SEP",          "({COMMENT}|{SPACE})*_",
            /** Operators **/
            "ADD_OP",       "<type=add_op><value:[-+]>{SEP}",
            "MUL_OP",       "<type=mul_op><value:[*/%]>{SEP}",
            "ASSIGN_OP",    "[-+*/%]?=",
            /** Keywords **/
            "IF",           "if![a-zA-Z_]{SEP}",
            "ELSE",         "else![a-zA-Z_]{SEP}",
            "RETURN",       "return![a-zA-Z_]{SEP}",
            "VOID",         "void![a-zA-Z_]{SEP}",
            "KEYWORD",      "{IF}|{ELSE}|{RETURN}|{VOID}",
            /** Symbols **/
            "LBRACK",       "\\{{SEP}",
            "RBRACK",       "\\}{SEP}",
            "LPAREN",       "\\({SEP}",
            "RPAREN",       "\\){SEP}",
            "SEMICOLON",    ";{SEP}",
            "ASSIGN",       "={SEP}",
            /** Values **/
            "IDENT",        "<type=ident>!({KEYWORD}|[0-9])"
                            "<value:[a-zA-Z0-9]+_>{SEP}",
            "INT",          "<type=int><value:"
                            "(0[xX][0-9a-fA-F]+_|[0-7]+_|!0[0-9]+_)>{SEP}",
            "CONSTANT",     "{INT}",
            /* Parser */
            "atom",         "{IDENT}|{CONSTANT}|{LPAREN}{expr}{RPAREN}",
            "sum",          "<type=sum><value:{atom}:(<op:{ADD_OP}>{atom})*>",
            "assign",       "<type=assign><value:{sum}:"
                            "(<op:{ASSIGN}><rhs:{sum}>)*>",
            "expr",         "<type=expr><value:{assign}:(,{assign})*>",
            "typename",     "<type=typename>(<name:{IDENT}>|{VOID}<name:void>)",
            "var_dec",      "<type=var_dec><var_type:{typename}><value:{expr}>",
            "return",       "<type=return>{RETURN}<value:{expr}>",
            "statement",    "{var_dec}{SEMICOLON}|{return}{SEMICOLON}"
                            "|{expr}{SEMICOLON}|{select}|{compound}",
            "compound",     "<type=compound>{LBRACK}<value:{statement}*>"
                            "{RBRACK}",
            "select",       "<type=if>{IF}LPAREN<cond:{expr}>{RPAREN}"
                            "<if:{statement}>({ELSE}<else:{statement}>)?",
            "args",         "{LPAREN}<arglist:(<typename:{typename}>"
                            "<argname:{IDENT}>?):(<typename:{typename}>"
                            "<argname:{IDENT}>?)*>?{RPAREN}",
            "function",     "<return_type:{typename}><fname:{IDENT}>"
                            "<arg:{args}>(<type=function>"
                            "<value:{compound}>|<type=fdec>{SEMICOLON})",
            "program",      "<type=program>{SEP}<value:{function}*>!"
        );
    return *PParser;
}
