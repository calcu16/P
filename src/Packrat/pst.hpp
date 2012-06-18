/*
Copyright (c) 2012, Andrew Carter, Dietrich Lagenbach, Xanda Schofield
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
#ifndef PST_HPP
#define PST_HPP

#include <string>
#include <list>
#include <tuple>
#include "../Wrapper/union.hpp"
#include "build_tree.hpp"

namespace packrat
{
    namespace pst
    {
        /*
         * Some things are recursively defined,
         *  and thus need to be wrapped in a struct
         */
        struct Expression;
        struct BinaryExpression;
        struct Index;
        struct UnaryExpression;
        struct Statement;
        struct Call;
        struct ForLoop;
        struct If;
        
        struct Type
        {
            enum type_t { TYPENAME };
            typedef wrapper::Union<std::string> type;
            static const int names_l = 1;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        
        
        typedef std::string Identifier;
        
        struct UnaryOp
        {
            typedef int type;
            enum op_t { NEGATE, COMPLEMENT, NOT, REFERENCE, DEREFERENCE };
            static const int names_l = 5;
            static const int prec = 12;
            typedef table_t<names_l>::type names_t;
            
            static names_t names;
            type value_;
        };
        
        struct BinOp
        {
            typedef int type;
            enum op_t { COMMA,
                        ASSIGN, PLUS_ASSIGN, MINUS_ASSIGN, TIMES_ASSIGN,
                        DIV_ASSIGN, MOD_ASSIGN, LSHIFT_ASSIGN, RSHIFT_ASSIGN,
                        AND_ASSIGN, XOR_ASSIGN, OR_ASSIGN,
                        LOR, LAND, OR, XOR, AND,
                        EQ, NEQ, LT, LEQ, GT, GEQ,
                        LSHIFT, RSHIFT, PLUS, MINUS, TIMES, DIVIDE, MOD };
            enum dir_t { L, R };
            
            static const int names_l = 30;
            typedef table_t<names_l>::type names_t;
            typedef array_t<int, names_l>::type prec_t;
            typedef array_t<dir_t, names_l>::type assoc_t;
            
            static prec_t prec;
            static assoc_t assoc;
            static names_t names;
            type value_;
        };
        
        
        struct Expression
        {
            enum expression_t { IDENTIFIER, INTEGER, UNARY,
                                BINARY, CALL, INDEX };
            typedef wrapper::Union<Identifier,
                                   unsigned long long int,
                                   UnaryExpression,
                                   BinaryExpression,
                                   Call,
                                   Index> type;
            static const int names_l = 6;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        
        typedef std::list<Expression> Arguments;
        
        struct Call
        {
            enum call_t { IDENTIFIER, ARGUMENTS };
            typedef std::tuple<Identifier, Arguments> type;
            static const int names_l = 2;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        
        struct Index
        {
            enum index_t {VALUE, INDEX};
            typedef postfix_t<Index, Expression, Expression> type;
            static const int names_l = 2;
            static const int prec = 13;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            std::tuple<Expression, Expression> value_;
            
            operator Expression() const;
        };
        
        struct BinaryExpression
        {
            enum binary_expression_t { LHS, OP, RHS };
            typedef fold_left_t<BinaryExpression, Expression, BinOp> type;
            static const int names_l = 2;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            std::tuple<Expression, BinOp, Expression> value_;
            
            operator Expression() const;
        };
        struct UnaryExpression
        {
            enum binary_expression_t { OP, VALUE };
            typedef std::tuple<UnaryOp, Expression> type;
            static const int names_l = 2;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            std::tuple<UnaryOp, Expression> value_;
        };
        
        
        
        struct Initializer
        {
            enum declaration_t { INDENTIFIER, VALUE };
            typedef std::tuple<Expression, Expression> type;
            static const int names_l = 2;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        
        struct Declaration
        {
            enum declaration_t { INITIALIZER, DEFAULT };
            typedef wrapper::Union<Initializer, Expression> type;
            static const int names_l = 2;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        
        struct Declarations
        {
            enum declarations_t { TYPE, DECLARATIONS };
            typedef std::tuple<Type, std::list<Declaration> > type;
            static const int names_l = 2;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        
        typedef std::list<Statement> Block;
        
        struct Statement
        {
            enum statement_t { SIMPLE, RETURN, DECLARATIONS, IF, FOR, BLOCK};
            static const int names_l = 6;
            typedef table_t<names_l>::type names_t;
            typedef wrapper::Union<Expression,
                                   Expression,
                                   Declarations,
                                   If,
                                   ForLoop,
                                   Block> type;
            static names_t names;
            type value_;
            
            operator Block() const;
        };
        
        struct ForLoop
        {
            enum forloop_t { INIT, COND, INC, BODY };
            typedef std::tuple<Expression, Expression,
                               Expression, Statement > type;
            static const int names_l = 4;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        
        struct If
        {
            enum if_t { COND, BODY };
            typedef std::tuple<Expression, Statement> type;
            static const int names_l = 2;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        
        
        struct Parameter
        {
            enum parameter_t { CONST, TYPE, NAME };
            static const int names_l = 3;
            typedef table_t<names_l>::type names_t;
            typedef std::tuple<bool, Type, Expression> type;
            static names_t names;
            type value_;
        };
        
        typedef std::list<Parameter> Parameters;
        
        struct Function
        {
            enum function_t { RETURN_TYPE, NAME, PARAMETERS, BODY };
            static const int names_l = 4;
            typedef table_t<names_l>::type names_t;
            typedef std::tuple<Type, Expression, Parameters, Block> type;
            static names_t names;
            type value_;
        };
        typedef std::list<Function> Program;
    }
}
#endif
