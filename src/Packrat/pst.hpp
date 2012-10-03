/*
Copyright (c) 2012,
Andrew Carter, Dietrich Langenbach, Rai Feren, Xanda Schofield

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
        
       /*
        * Defines a type (e.g, Simple)
        */
        struct Type
        {
            enum type_t { TYPENAME };
            typedef wrapper::Union<std::string> type;
            static const int names_l = 1;
            typedef table_t<names_l>::type names_t;
            static names_t names;  // Holds the names of all types for printing
            type value_; // Marks which type has been constructed
        };
        
        
        typedef std::string Identifier;

       /*
        * Defines Unary Operations.
        * 
        *  Unary Operators all have the same precedence for printing,
        *  which is greater than that of Binary Operators.
        */
        struct UnaryOp
        {
            typedef int type;
            enum op_t { NEGATE, COMPLEMENT, NOT, REFERENCE, DEREFERENCE };
            static const int names_l = 5;
            static const int prec = 12;  // Defines precedence for printing
            typedef table_t<names_l>::type names_t;
            static names_t names;  // Contains names of operators
            type value_; // Marks which UnaryOp has been constructed
        };
        
       /*
        * Defines Binary Operations
        * 
        *  Binary Operators can be Left or Right associative.
        *  Binary Operators also each have their own precedence.
        */
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
            
            static prec_t prec;  // Defines order for printing
            // Defines if assoc[i] is Left or Right associative
            static assoc_t assoc;
            static names_t names;  // Defines how to print names[i]
            type value_; // Marks what type of BinOp has been constructed
        };
        
        
       /*
        * Expressions 
        * 
        *   Expressions are essentially operators paired with values
        *   (the values themselves can be more expressions)
        */
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
            static names_t names; // Defines how to print Expressions
            type value_; // Marks which Expression was constructed
        };
        
        typedef std::list<Expression> Arguments;
        
       /*
        * Call
        *
        *  A Call identifies either the function being called or what
        *  arguments are passed to a function.
        */
        struct Call
        {
            enum call_t { IDENTIFIER, ARGUMENTS };
            typedef std::tuple<Identifier, Arguments> type;
            static const int names_l = 2;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        

       /*
        * Index 
        *
        *  Indexes are used to access specific elements of a
        *  Container.
        */
        struct Index
        {
            enum index_t {VALUE, INDEX};
            typedef postfix_t<Index, Expression, Expression> type;
            static const int names_l = 2;
            static const int prec = 13;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            // Often Indexes are calculated from something, so our
            // value is the result of expressions.
            std::tuple<Expression, Expression> value_;
            
            operator Expression() const;
        };
        
       /*
        * Binary Expressions look like x + (y * 2). (LHS = x, RHS is
        * another Expression, y * 2)
        */
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
       /*
        * UnaryExpressions look like !(x || y). (OP = !, VALUE = (x || y))
        */
        struct UnaryExpression
        {
            enum binary_expression_t { OP, VALUE };
            typedef std::tuple<UnaryOp, Expression> type;
            static const int names_l = 2;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            std::tuple<UnaryOp, Expression> value_;
        };
        
        
       /*
        * Initializers connect a Variable Identifier to some Value.
        */
        struct Initializer
        {
            enum declaration_t { INDENTIFIER, VALUE };
            typedef std::tuple<Expression, Expression> type;
            static const int names_l = 2;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        
       /*
        * Declarations of variables either Initialize to a value, or
        * go with the default.
        */
        struct Declaration
        {
            enum declaration_t { INITIALIZER, DEFAULT };
            typedef wrapper::Union<Initializer, Expression> type;
            static const int names_l = 2;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            // Is this variable using an Initializer or the Default?
            type value_;
        };
        
       /*
        * Can put multiple declarations of variables on a line.
        */
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

       /*
        * Statements are either expressions, declarations of
        * variables, or control flow commands (If/For)
        */
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
        
       /*
        * For loops create a loop counter variable, check for some
        * condition, increment their loop counter in some way, and
        * each iteration execute the Body.
        */
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
        
       /*
        * If statements check a condition. If the condition is true,
        * then execute the body. Need to track both of those.
        */
        struct If
        {
            enum if_t { COND, BODY };
            typedef std::tuple<Expression, Statement> type;
            static const int names_l = 2;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        
       /*
        * Parameters are a type paired with a name with an additional
        * specifier of if they are `const` or not. These get passed
        * into functions.
        */
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
        
       /*
        * Functions are defined by four things: Return Type, Name,
        * Parameter List, Body.
        *
        * e.g. Int foo(char a) { return 42; }
        */
        struct Function
        {
            enum function_t { RETURN_TYPE, NAME, PARAMETERS, BODY };
            static const int names_l = 4;
            typedef table_t<names_l>::type names_t;
            typedef std::tuple<Type, Identifier, Parameters, Block> type;
            static names_t names;
            type value_;
        };
        typedef std::list<Function> Program;
    }
}
#endif
