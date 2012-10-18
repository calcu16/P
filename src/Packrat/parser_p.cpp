/*
Copyright (c) 2012, Andrew Carter, Dietrich Langenbach, Xanda Schofield
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
*/
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
            "POST_OP",      "<value:\\+\\+|\\-\\->{SEP}",
            "UN_OP",        "<value:[-+*&~]>{SEP}",
            "ADD_OP",       "<value:[-+]>{SEP}",
            "MUL_OP",       "<value:[*/%]>{SEP}",
            "ASSIGN_OP",    "<value:[-+*/%]?=>{SEP}",
            "REL_OP",       "<value:[<>!=]=|\\<|\\>>{SEP}",
            "COMMA_OP",     "<value:,>{SEP}",
            /** Keywords **/
            "IF",           "if![a-zA-Z_]{SEP}",
            "ELSE",         "else![a-zA-Z_]{SEP}",
            "RETURN",       "return![a-zA-Z_]{SEP}",
            "FOR",          "for![a-zA-Z_]{SEP}",
            "VOID",         "<value:void>![a-zA-Z_]{SEP}",
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
            /** Values **/
            "IDENT",        "<value:"
                              "<type=Ident>!({KEYWORD}|[0-9])"
                              "<value:[a-zA-Z0-9]+_>{SEP}"
                            ">",
            "INT",          "<type=int><value:"
                            "(0[xX][0-9a-fA-F]+_|[0-7]+_|!0[0-9]+_)>{SEP}",
            "CONSTANT",     "{INT}",
            /* Parser */
            "typename",     "<value:<type=Simple>"
                                "({IDENT}|{VOID})>",
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
                                "<Op:{UN_OP}><Expression:{maybe_unary}>"
                            ">",
            "maybe_unary",  "<value:{unary}>|{maybe_index}",
            "prod",         "<type=BinaryExpr><value:"
                                "<Value:{maybe_unary}>:"
                                "(<Op:{MUL_OP}><Value:{maybe_unary}>)+"
                            ">",
            "maybe_prod",   "<value:{prod}>|{maybe_unary}",
            "sum",          "<type=BinaryExpr><value:"
                                "<Value:{maybe_prod}>:"
                                "(<Op:{ADD_OP}><Value:{maybe_prod}>)+"
                            ">",
            "maybe_sum",    "<value:{sum}>|{maybe_prod}",
            "rel",          "<type=BinaryExpr><value:"
                                "<Value:{maybe_sum}>"
                                ":(<Op:{REL_OP}><Value:{maybe_sum}>)+"
                            ">",
            "maybe_rel",    "<value:{rel}>|{maybe_sum}",
            "assign",       "<type=BinaryExpr><value:"
                                "<Value:{maybe_rel}>:"
                                "(<Op:{ASSIGN_OP}><Value:{maybe_assign}>)"
                            ">",
            "maybe_assign", "<value:{assign}>|{maybe_rel}",
            "ignore_comma", "{maybe_assign}",
            "comma",        "<type=BinaryExpr><value:"
                                "<Value:{maybe_assign}>"
                                ":(<Op:{COMMA_OP}><Value:{maybe_assign}>)+"
                            ">",    
            "maybe_comma",  "<value:{comma}>|{maybe_assign}",
            "expression",   "{maybe_comma}",
            "initializer",  "<value:"
                                "<Name:{maybe_unary}>"
                                "{ASSIGN}"
                                "<Value:{ignore_comma}>"
                            ">",
            "declaration",  "<value:"
                                "(<type=Init>{initializer})"
                                "|(<type=Default>{ignore_comma})"
                            ">",
            "decls",        "<Type:{typename}>"
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
                                "<Type:{typename}><Name:{maybe_unary}>"
                            ">",
            "parameters",   "{LPAREN}"
                                "<value:({parameter}:({COMMA}{parameter})*)|>"
                            "{RPAREN}",
            "empty_par",    "{LPAREN}{VOID}{RPAREN}<value=>",
            "function",     "<value:"
                                "<ReturnType:{typename}>"
                                "<Name:{IDENT}>"
                                "<Pars:{parameters}|{empty_par}>"
                                "<Block:{block}>"
                            ">",
            "program",      "{SEP}<value:{function}*>!"
        );
    return *PParser;
}
