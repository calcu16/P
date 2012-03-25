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
        struct Block;
        
        struct Type
        {
            typedef wrapper::Union<std::string> type;
            static const int names_l = 1;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        
        
        typedef std::string Identifier;
        
        /*
        enum unary_t { NEGATE, COMPLEMENT, NOT };
        typedef std::tuple<unary_t, Expression> UnaryExpression;
        */
        struct BinOp
        {
            typedef int type;
            enum op_t { ASSIGN, PLUS, MINUS };
            static const int names_l = 3;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        
        
        enum expression_t { IDENTIFIER, UNARY, BINARY };
        struct Expression
        {
            typedef wrapper::Union<Identifier,
                                   /*UnaryExpression,*/
                                   BinaryExpression> type;
            static const int names_l = 2;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            type value_;
        };
        
        
        struct BinaryExpression
        {
            typedef fold_left_t<BinaryExpression, Expression, BinOp> type;
            static const int names_l = 2;
            typedef table_t<names_l>::type names_t;
            static names_t names;
            std::tuple<Expression, BinOp, Expression> value_;
            
            operator Expression() const;
        };
        
        
        
        
        enum statement_t { SIMPLE, RETURN, BLOCK};
        struct Statement
        {
            static const int names_l = 3;
            typedef table_t<names_l>::type names_t;
            typedef wrapper::Union<Expression,
                                   Expression,
                                   Block> type;
            static names_t names;
            type value_;
        };
        
        
        struct Block
        {
            typedef std::list<Statement> type;
            type value_;
        };
    }
}
#endif
