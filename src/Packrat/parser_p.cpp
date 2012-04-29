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
            "CONST",        "const![a-zA-Z_]{SEP}",
            "KEYWORD",      "{IF}|{ELSE}|{RETURN}|{VOID}|{FOR}",
            /** Symbols **/
            "LBRACK",       "\\{{SEP}",
            "RBRACK",       "\\}{SEP}",
            "LPAREN",       "\\({SEP}",
            "RPAREN",       "\\){SEP}",
            "LSQUARE",      "\\[{SEP}",
            "RSQUARE",      "\\]{SEP}",
            "SEMICOLON",    ";{SEP}",
            "COMMA",        ",{SEP}",
            "ASSIGN",       "={SEP}",
            "RELOP",        "<value:\\=\\=|\\<\\=|\\!\\=|\\>\\=|\\<|\\>>{SEP}",
            /** Values **/
            "IDENT",        "<type=Ident>!({KEYWORD}|[0-9])"
                            "<value:[a-zA-Z0-9]+_>{SEP}",
            "INT",          "<type=int><value:"
                            "(0[xX][0-9a-fA-F]+_|[0-7]+_|!0[0-9]+_)>{SEP}",
            "CONSTANT",     "{INT}",
            /* Parser */
            "typename",     "<type=Simple><value:{IDENT}|{VOID}>",
            "atom",         "{LPAREN}{expression}{RPAREN}|"
                            "<value:<type=Ident><value:{IDENT}>|"
                            "<type=Int><value:{CONSTANT}>>",
            "args",         "{ignore_comma}:({COMMA}{ignore_comma})*|",
            "call",         "<type=Call><value:"
                                "<Function:{IDENT}>"
                                "{LPAREN}"
                                    "<Arguments:{args}>"
                                "{RPAREN}"
                            ">",
            "maybe_call",   "<value:{call}>|{atom}",
            "index",        "<type=Index><value:"
                                "<Value:{maybe_call}>"
                                "<Index:({LSQUARE}{expression}{RSQUARE})+>"
                            ">",
            "maybe_index",  "<value:{index}>|{maybe_call}",
            "unary",        "<type=UnaryExpr><value:"
                                "<Op:<value:[-~!&*]>>{SEP}"
                                    "<Expression:{maybe_unary}>"
                            ">",
            "maybe_unary",  "<value:{unary}>|{maybe_index}",
            "prod",         "<type=BinaryExpr><value:"
                                "<Value:{maybe_unary}>:"
                                "(<Op:<value:[*/%]>>{SEP}"
                                    "<Value:{maybe_unary}>)+"
                            ">",
            "maybe_prod",   "<value:{prod}>|{maybe_unary}",
            "sum",          "<type=BinaryExpr><value:"
                                "<Value:{maybe_prod}>:"
                                "(<Op:<value:[+-]>>{SEP}<Value:{maybe_prod}>)+"
                            ">",
            "maybe_sum",    "<value:{sum}>|{maybe_prod}",
            "rel",          "<type=BinaryExpr><value:"
                                "<Value:{maybe_sum}>"
                                ":(<Op:{RELOP}><Value:{maybe_sum}>)+"
                            ">",
            "maybe_rel",    "<value:{rel}>|{maybe_sum}",
            "assign",       "<type=BinaryExpr><value:"
                                "<Value:{maybe_rel}>:"
                                "(<Op:<value:[=]>>{SEP}<Value:{maybe_assign}>)"
                            ">",
            "maybe_assign", "<value:{assign}>|{maybe_rel}",
            "ignore_comma", "{maybe_assign}",
            "expression",   "{maybe_assign}",
            "initializer",  "<value:"
                                "<Name:{maybe_unary}>"
                                "{ASSIGN}"
                                "<Value:{ignore_comma}>"
                            ">",
            "declaration",  "<value:"
                                "(<type=Init>{initializer})"
                                "|(<type=Default>{ignore_comma})"
                            ">",
            "decls",        "<Type:<value:{typename}>>"
                                "<Decls:<value:{declaration}:"
                                    "({COMMA}{declaration})*>>",
            "for_loop",     "{FOR}{LPAREN}"
                                "<Init:{expression}>{SEMICOLON}"
                                "<Cond:{expression}>{SEMICOLON}"
                                "<Inc:{expression}>{RPAREN}"
                            "<Body:{statement}>",
            "if",           "{IF}{LPAREN}<Cond:{expression}>{RPAREN}"
                                "<Body:{statement}>",
            "statement",    "<value:"
                                "(<type=Simple>{expression}{SEMICOLON})|"
                                "(<type=Decls><value:{decls}>{SEMICOLON})|"
                                "(<type=Ret>{RETURN}{expression}{SEMICOLON})|"
                                "(<type=For><value:{for_loop}>)|"
                                "(<type=If><value:{if}>)|"
                                "(<type=Block>{block})"
                            ">",
            "block",        "{LBRACK}<value:{statement}*>{RBRACK}",
            "parameter",    "<value:"
                                "<Const:<value:{CONST}?>>"
                                "<Type:<value:{typename}>><Name:{expression}>"
                            ">",
            "parameters",   "{LPAREN}"
                                "<value:({parameter}:({COMMA}{parameter})*)|>"
                            "{RPAREN}",
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
