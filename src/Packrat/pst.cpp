#include "pst.hpp"
using namespace std;
using namespace packrat;
using namespace packrat::pst;


Type::names_t Type::names = {"Type"};
BinOp::names_t BinOp::names = {"=","+","-"};
Expression::names_t Expression::names = {"Ident", "BinaryExpr"};
BinaryExpression::names_t BinaryExpression::names = {"value", "sep"};
Statement::names_t Statement::names = {"Simple", "Return", "Block"};

BinaryExpression::operator Expression() const
{
    Expression expr;
    expr.value_.set<1>(*this);
    return expr;
}
