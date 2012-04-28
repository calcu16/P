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
        struct UnaryExpression;
        struct Statement;
        struct Call;
        struct ForLoop;
        
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
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        
        struct BinOp
        {
            typedef int type;
            enum op_t { ASSIGN, PLUS, MINUS, TIMES, DIVIDE };
            static const int names_l = 5;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        
        
        struct Expression
        {
            enum expression_t { IDENTIFIER, INTEGER, UNARY, BINARY, CALL };
            typedef wrapper::Union<Identifier,
                                   unsigned long long int,
                                   UnaryExpression,
                                   BinaryExpression,
                                   Call> type;
            static const int names_l = 5;
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
            enum statement_t { SIMPLE, RETURN, DECLARATIONS, FOR, BLOCK};
            static const int names_l = 5;
            typedef table_t<names_l>::type names_t;
            typedef wrapper::Union<Expression,
                                   Expression,
                                   Declarations,
                                   ForLoop,
                                   Block> type;
            static names_t names;
            type value_;
        };
        
        struct ForLoop
        {
            enum forloop_t { INIT, COND, INC, BODY };
            typedef std::tuple<Expression, Expression, Expression, Statement> type;
            static const int names_l = 4;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        
        struct Parameter
        {
            enum parameter_t { TYPE, NAME };
            static const int names_l = 2;
            typedef table_t<names_l>::type names_t;
            typedef std::tuple<Type, Identifier> type;
            static names_t names;
            type value_;
        };
        
        typedef std::list<Parameter> Parameters;
        
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
