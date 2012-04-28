#include "pst.hpp"
using namespace std;
using namespace packrat;
using namespace packrat::pst;


Type::names_t               Type::names                 = {"Simple"};
UnaryOp::names_t            UnaryOp::names              = {"-","~","!","&","*"};
BinOp::names_t              BinOp::names                = {"=","+","-","*","/"};
Expression::names_t         Expression::names           = {"Ident", "Int", "UnaryExpr", "BinaryExpr", "Call"};
Call::names_t               Call::names                 = {"Function","Arguments"};
UnaryExpression::names_t    UnaryExpression::names      = {"Op", "Expression"};
BinaryExpression::names_t   BinaryExpression::names     = {"Value", "Op"};
Initializer::names_t        Initializer::names          = {"Name", "Value"};
Declaration::names_t        Declaration::names          = {"Init", "Default"};
Declarations::names_t       Declarations::names         = {"Type", "Decls"};
Statement::names_t          Statement::names            = {"Simple", "Return", "Declarations", "For", "Block"};
ForLoop::names_t            ForLoop::names              = {"Init", "Cond", "Inc", "Body"};
Parameter::names_t          Parameter::names            = {"Type", "Name"};
Function::names_t           Function::names             = {"ReturnType", "Name", "Pars", "Block"};

BinaryExpression::operator Expression() const
{
    Expression expr;
    expr.value_.set<Expression::BINARY>(*this);
    return expr;
}
