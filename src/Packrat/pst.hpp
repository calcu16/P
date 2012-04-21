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
            enum op_t { ASSIGN, PLUS, MINUS };
            static const int names_l = 5;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        
        
        struct Expression
        {
            enum expression_t { IDENTIFIER, UNARY, BINARY };
            typedef wrapper::Union<Identifier,
                                   UnaryExpression,
                                   BinaryExpression> type;
            static const int names_l = 3;
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
            typedef std::tuple<UnaryOp, Expression> type;
            static const int names_l = 2;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            std::tuple<UnaryOp, Expression> value_;
        };
        
        struct Statement;
        typedef std::list<Statement> Block;
        
        struct Statement
        {
            enum statement_t { SIMPLE, RETURN, BLOCK};
            static const int names_l = 3;
            typedef table_t<names_l>::type names_t;
            typedef wrapper::Union<Expression,
                                   Expression,
                                   Block> type;
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
