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
#include "pst.hpp"
using namespace std;
using namespace packrat;
using namespace packrat::pst;


Type::names_t               Type::names
    = {"Simple"};
UnaryOp::names_t            UnaryOp::names
    = {"-","~","!","&","*"};

BinOp::prec_t               BinOp::prec
    = { 0 ,
        1 , 1 , 1 , 1 , 1 , 1 ,
        1 , 1 , 1 , 1 , 1 ,
        2 , 3 , 4 , 5 , 6 ,
        7 , 7 , 8 , 8 , 8 , 8 ,
        9 , 9 , 10, 10, 11, 11, 11,
      };
BinOp::assoc_t              BinOp::assoc
    = { L ,
        R , R , R , R , R , R ,
        R , R , R , R , R ,
        L , L , L , L , L ,
        L , L , L , L , L , L ,
        L , L , L , L , L , L , L ,
      };
BinOp::names_t              BinOp::names
    = { ",",
       "=", "+=", "-=", "*=", "/=", "%=",
       "<<=", ">>=", "&=", "^=", "|=",
       "||", "&&", "|", "^", "&",
       "==", "!=", "<", "<=", ">", ">=",
       "<<", ">>", "+", "-", "*", "/", "%"
      };

Expression::names_t         Expression::names
    = {"Ident", "Int", "UnaryExpr", "BinaryExpr", "Call", "Index"};
Call::names_t               Call::names
    = {"Function","Arguments"};
UnaryExpression::names_t    UnaryExpression::names
    = {"Op", "Expression"};
BinaryExpression::names_t   BinaryExpression::names
    = {"Value", "Op"};
Index::names_t              Index::names
    = {"Value", "Index"};
Initializer::names_t        Initializer::names
    = {"Name", "Value"};
Declaration::names_t        Declaration::names
    = {"Init", "Default"};
Declarations::names_t       Declarations::names
    = {"Type", "Decls"};
Statement::names_t          Statement::names
    = {"Simple", "Ret", "Decls", "If", "For", "Block"};
ForLoop::names_t            ForLoop::names
    = {"Init", "Cond", "Inc", "Body"};
If::names_t                 If::names
    = {"Cond", "Body"};
Parameter::names_t          Parameter::names
    = {"Const", "Type", "Name"};
Function::names_t           Function::names
    = {"ReturnType", "Name", "Pars", "Block"};

BinaryExpression::operator Expression() const
{
    Expression expr;
    expr.value_.set<Expression::BINARY>(*this);
    return expr;
}

Index::operator Expression() const
{
    Expression expr;
    expr.value_.set<Expression::INDEX>(*this);
    return expr;
}

Statement::operator Block() const
{
    if((int)value_ == BLOCK)
        return value_.get<BLOCK>();
    return Block(1, *this);
}
