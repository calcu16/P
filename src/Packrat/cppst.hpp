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
