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
        
        typedef wrapper::Union<std::string> Type;
        
        typedef std::string Identifier;
        
        enum unary_t { NEGATE, COMPLEMENT, NOT };
        typedef std::tuple<unary_t, Expression> UnaryExpression;
        
        enum binary_t { ASSIGN, PLUS, MINUS };
        typedef std::tuple<binary_t, Expression, Expression> BinaryExpression;
        
        enum expression_t { IDENTIFIER, UNARY, BINARY };
        struct Expression
        {
            typedef strings<"Ident","UnaryExpr","BinaryExpr">;
            typedef wrapper::Union<Identifier, UnaryExpression, BinaryExpression> type;
            type value_;
        };
        
        enum statement_t { SIMPLE, RETURN, BLOCK};
        typedef wrapper::Union<Expression, Expression, Block> Statement;
        
        struct Block
        {
            typedef std::list<Statement> type;
            type value_;
        };
        
        
    }
}
#endif
