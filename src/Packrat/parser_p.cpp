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
            "UN_OP",        "<type=un_op><value:[-+*&~]>{SEP}",
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
            "IDENT",        "<type=Ident>!({KEYWORD}|[0-9])"
                            "<value:[a-zA-Z0-9]+_>{SEP}",
            "INT",          "<type=int><value:"
                            "(0[xX][0-9a-fA-F]+_|[0-7]+_|!0[0-9]+_)>{SEP}",
            "CONSTANT",     "{INT}",
            /* Parser */
            "typename",     "<type=Simple><value:{IDENT}|{VOID}>",
            "atom",         "{LPAREN}{expression}{RPAREN}|"
                            "<type=Ident><value:{IDENT}>",
            "unary",        "<type=UnaryExpr><value:"
                                "<Op:[-~!&*]>{SEP}<Expression:{maybe_unary}>"
                            ">",
            "maybe_unary",  "{unary}|{atom}",
            "prod",         "(<type=BinaryExpr><value:"
                                "<Value:{maybe_unary}>"
                                ":(<Op:[*/]>{SEP}<Value:{maybe_unary}>)+"
                            ">)",
            "maybe_prod",   "{prod}|{atom}",
            "sum",          "<type=BinaryExpr><value:"
                                "<Value:{maybe_prod}>"
                                ":(<Op:[+-]>{SEP}<Value:{maybe_prod}>)+"
                            ">",
            "maybe_sum",    "{sum}|{maybe_prod}",
            "assign",       "(<type=BinaryExpr><value:"
                                "<Value:{maybe_sum}>"
                                ":(<Op:[=]>{SEP}<Value:{maybe_assign}>)"
                            ">)",
            "maybe_assign", "<value:{assign}>|{maybe_sum}",
            "expression",   "{maybe_assign}",
            "statement",    "<value:"
                                "(<type=Simple><value:{expression}>{SEMICOLON})|"
                                "(<type=Return>{RETURN}<value:{expression}>{SEMICOLON})|"
                                "(<type=Block><value:{block}>)"
                            ">",
            "block",        "{LBRACK}<value:{statement}*>{RBRACK}",
            "type",         "<value:(<type=Simple><value:{IDENT}>)>",
            "parameter",    "<value:<Type:{type}><Name:{IDENT}>>",
            "parameters",   "{LPAREN}<value:|{parameter}:({comma}{parameter})+>{RPAREN}",
            "function",     "<value:"
                                "<ReturnType:<value:{typename}>>"
                                "<Name:{IDENT}>"
                                "<Pars:{parameters}>"
                                "<Block:{block}>"
                            ">",
            "program",      "{SEP}<value:{function}*>!"
        );
    return *PParser;
}
