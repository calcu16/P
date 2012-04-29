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
        7 , 7 , 8 , 8 , 8 , 8 , 8 ,
        9 , 9 , 10, 10, 11, 11, 11,
      };
BinOp::assoc_t              BinOp::assoc
    = { L ,
        R , R , R , R , R , R ,
        R , R , R , R , R ,
        L , L , L , L , L ,
        L , L , L , L , L , L , L ,
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
