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
        struct Block;
        
        struct Type
        {
            typedef wrapper::Union<std::string> type;
            static const int count = 1;
            static table_t<count>::type names;
            type value_;
        };
        table_t<Type::count>::type Type::names = {"Type"};
        
        
        typedef std::string Identifier;
        
        enum unary_t { NEGATE, COMPLEMENT, NOT };
        typedef std::tuple<unary_t, Expression> UnaryExpression;
        
        enum binary_t { ASSIGN, PLUS, MINUS };
        typedef std::tuple<binary_t, Expression, Expression> BinaryExpression;
        
        enum expression_t { IDENTIFIER, UNARY, BINARY };
        struct Expression
        {
            static table_t<3>::type names;
            typedef wrapper::Union<Identifier, UnaryExpression, BinaryExpression> type;
            type value_;
        };
        // table_t<3>::type Expression::names = new std::string[]("Ident", "UnaryExpr", "BinaryExpr");
        
        enum statement_t { SIMPLE, RETURN, BLOCK};
        struct Statement
        {
            static table_t<3>::type names;
            typedef wrapper::Union<Expression, Expression, Block> type;
            type value_;
        };
        // table_t<3>::type Statement::names = {"Simple", "Return", "Block"};
        
        struct Block
        {
            typedef std::list<Statement> type;
            type value_;
        };
    }
}
#endif
