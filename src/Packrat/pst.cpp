#include "pst.hpp"
using namespace std;
using namespace packrat;
using namespace packrat::pst;


Type::names_t               Type::names                 = {"Simple"};
UnaryOp::names_t            UnaryOp::names              = {"-","~","!","&","*"};
BinOp::names_t              BinOp::names                = {"=","+","-","*","/"};
Expression::names_t         Expression::names           = {"Ident", "UnaryExpr", "BinaryExpr"};
UnaryExpression::names_t    UnaryExpression::names      = {"Op", "Expression"};
BinaryExpression::names_t   BinaryExpression::names     = {"Value", "Op"};
Statement::names_t          Statement::names            = {"Simple", "Return", "Block"};
Parameter::names_t          Parameter::names            = {"Type", "Name"};
Function::names_t           Function::names             = {"ReturnType", "Name", "Pars", "Block"};

BinaryExpression::operator Expression() const
{
    Expression expr;
    expr.value_.set<2>(*this);
    return expr;
}
