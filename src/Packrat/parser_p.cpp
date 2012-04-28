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
            "FOR",          "for![a-zA-Z_]{SEP}",
            "VOID",         "void![a-zA-Z_]{SEP}",
            "KEYWORD",      "{IF}|{ELSE}|{RETURN}|{VOID}|{FOR}",
            /** Symbols **/
            "LBRACK",       "\\{{SEP}",
            "RBRACK",       "\\}{SEP}",
            "LPAREN",       "\\({SEP}",
            "RPAREN",       "\\){SEP}",
            "LSQUARE",      "\\({SEP}",
            "RSQUARE",      "\\){SEP}",
            "SEMICOLON",    ";{SEP}",
            "COMMA",        ",{SEP}",
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
                            "<type=Ident><value:{IDENT}>|"
                            "<type=Int><value:{CONSTANT}>",
            "args",         "{ignore_comma}:({COMMA}{ignore_comma})*|",
            "call",         "<type=Call><value:"
                                "<Function:{IDENT}>"
                                "{LPAREN}"
                                    "<Arguments:<value:{args}>>"
                                "{RPAREN}"
                            ">",
            "maybe_call",   "{call}|{atom}",
            "index",        "<type=Index><value:"
                                "<Value:<value:{maybe_call}>>"
                                "({LSQUARE}<Op:<value:{expression}>>{RSQUARE})+"
                            ">",
            "maybe_index",  "{index}|{maybe_call}",
            "unary",        "<type=UnaryExpr><value:"
                                "<Op:<value:[-~!&*]>>{SEP}<Expression:<value:{maybe_unary}>>"
                            ">",
            "maybe_unary",  "{unary}|{maybe_index}",
            "prod",         "<type=BinaryExpr><value:"
                                "<Value:<value:{maybe_unary}>>"
                                ":(<Op:<value:[*/]>>{SEP}<Value:{maybe_unary}>)+"
                            ">",
            "maybe_prod",   "{prod}|{maybe_unary}",
            "sum",          "<type=BinaryExpr><value:"
                                "<Value:<value:{maybe_prod}>>"
                                ":(<Op:<value:[+-]>>{SEP}<Value:<value:{maybe_prod}>>)+"
                            ">",
            "maybe_sum",    "{sum}|{maybe_prod}",
            "assign",       "<type=BinaryExpr><value:"
                                "<Value:<value:{maybe_sum}>>"
                                ":(<Op:<value:[=]>>{SEP}<Value:<value:{maybe_assign}>>)"
                            ">",
            "maybe_assign", "{assign}|{maybe_sum}",
            "ignore_comma", "{maybe_assign}",
            "expression",   "{maybe_assign}",
            "initializer",  "<Name:<value:{maybe_unary}>>"
                            "{ASSIGN}"
                            "<Value:<value:{ignore_comma}>>",
            "declaration",  "<value:"
                                "(<type=Init><value:{initializer}>)"
                                "|(<type=Default><value:{ignore_comma}>)"
                            ">",
            "declarations", "<Type:<value:{typename}>>"
                                "<Decls:<value:{declaration}:({COMMA}{declaration})*>>",
            "for_loop",     "{FOR}{LPAREN}"
                                "<Init:<value:{expression}>>{SEMICOLON}"
                                "<Cond:<value:{expression}>>{SEMICOLON}"
                                "<Inc:<value:{expression}>>{RPAREN}"
                            "<Body:<value:{statement}>>",
            "statement",    "<value:"
                                "(<type=Simple><value:{expression}>{SEMICOLON})|"
                                "(<type=Declarations><value:{declarations}>{SEMICOLON})|"
                                "(<type=Return>{RETURN}<value:{expression}>{SEMICOLON})|"
                                "(<type=For><value:{for_loop}>)|"
                                "(<type=Block><value:{block}>)"
                            ">",
            "block",        "{LBRACK}<value:{statement}*>{RBRACK}",
            "type",         "<value:(<type=Simple><value:{IDENT}>)>",
            "parameter",    "<value:<Type:{type}><Name:{IDENT}>>",
            "parameters",   "{LPAREN}<value:({parameter}:({COMMA}{parameter})*)|>{RPAREN}",
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
