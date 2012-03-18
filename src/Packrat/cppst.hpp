#ifndef CPPST_HPP
#define CPPST_HPP

#include <string>
#include <list>
#include <tuple>
#include "../Wrapper/union.hpp"

namespace Packrat
{
    namespace CPPST
    {
        /*
         * Some things are recursively defined,
         *  and thus need to be wrapped in a struct
         */
        struct Expression;
        struct Block;
        
        typedef std::string Identifier;
        
        enum unary_t { NEGATE, COMPLEMENT, NOT };
        typedef std::tuple<unary_t, Expression> UnaryExpression;
        
        enum binary_t { ASSIGN, PLUS, MINUS };
        typedef std::tuple<binary_t, Expression, Expression> BinaryExpression;
        
        enum expression_t { IDENTIFIER, UNARY, BINARY };
        struct Expression
        {
            wrapper::Union<Identifier, UnaryExpression, BinaryExpression> _;
        };
        
        enum statement_t { SIMPLE, RETURN, BLOCK};
        typedef wrapper::Union<Expression, Expression> Statement;
        
        struct Block { std::list<Statement>  _; };
        
    }
}

#endif
