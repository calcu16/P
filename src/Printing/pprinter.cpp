#include "pprinter.hpp"
#include "../Packrat/pst.hpp"
#include <iostream>
#include <tuple>

using namespace packrat;
using namespace pst;
static ostream& operator<<(ostream&, const Statement&);
static ostream& operator<<(ostream&, const Expression&);
bool IS_NEWLINE = true;
int INDENT_NO = 0;

static ostream& operator<<(ostream& out, const Type& type)
{
    assert((int)type.value_ != -1);
    switch(type.value_)
    {
    case Type::TYPENAME:
        indentedOutput(out, type.value_.get<Type::TYPENAME>());
        break;
    default:
        assert(0);
    }
    return out;
}

static ostream& operator<<(ostream& out, const BinOp& op)
{
    switch(op.value_)
    {
    case BinOp::ASSIGN:
        return out << " = ";
    case BinOp::PLUS:
        return out << " + ";
    case BinOp::MINUS:
        return out << " - ";
    default:
        assert(0);
    }
}

static ostream& operator<<(ostream& out, const Arguments& arguments)
{
    out << "(";
    for(Arguments::const_iterator i = arguments.begin(); i != arguments.end(); )
    {
        out << *i;
        if(++i != arguments.end())
            out << ",";
    }
    out << ")";
}

static ostream& operator<<(ostream& out, const Call& call)
{
    return out  << get<Call::IDENTIFIER>(call.value_)
                << get<Call::ARGUMENTS>(call.value_);
}

static ostream& operator<<(ostream& out, const BinaryExpression& bexpr)
{
    return out  << get<BinaryExpression::LHS>(bexpr.value_)
                << get<BinaryExpression::OP >(bexpr.value_)
                << get<BinaryExpression::RHS>(bexpr.value_);
}

static ostream& operator<<(ostream& out, const Expression& expr)
{
    assert((int)expr.value_ != -1);
    switch(expr.value_)
    {
    case Expression::IDENTIFIER:
        indentedOutput(out, expr.value_.get<Expression::IDENTIFIER>());
        break;
    case Expression::UNARY:
        indentedOutput(out, "un");
        break;
    case Expression::BINARY:
        indentedOutput(out, expr.value_.get<Expression::BINARY>());
        break;
    case Expression::CALL:
        indentedOutput(out, expr.value_.get<Expression::CALL>());
        break;
    default:
        assert(0);
    }
    return out;
}

static ostream& operator<<(ostream& out, const Block& block)
{
    indentedOutput(out, "{\n");
    ++INDENT_NO;
    for(Block::const_iterator i = block.begin(); i != block.end(); ++i) {
        indentedOutput(out, *i);
        indentedOutput(out, "\n");
    }
    --INDENT_NO;
    indentedOutput(out, "}\n");
    return out;
}

static ostream& operator<<(ostream& out, const Statement& statement)
{
    assert((int)statement.value_ != -1);
    switch(statement.value_)
    {
    case Statement::RETURN:
        indentedOutput(out, "return ");
        indentedOutput(out, statement.value_.get<Statement::RETURN>());
        indentedOutput(out, ";");
        break;
    case Statement::SIMPLE:
        indentedOutput(out, statement.value_.get<Statement::SIMPLE>());
        indentedOutput(out, ";");
        break;
    case Statement::BLOCK:
        indentedOutput(out, statement.value_.get<Statement::BLOCK>());
        indentedOutput(out, ";");
        break;
    default:
        assert(0);
    }
    return out;
}

static ostream& operator<<(ostream& out, const Parameter& par)
{
    indentedOutput(out, get<Parameter::TYPE>(par.value_));
    indentedOutput(out, " ");
    indentedOutput(out, get<Parameter::NAME>(par.value_));
    return out;
}

static ostream& operator<<(ostream& out, const Parameters& pars)
{
    Parameters::const_iterator i = pars.begin();
    indentedOutput(out, "(");
    if(i != pars.end()) for(;;)
    {
        indentedOutput(out, *i);
        if(++i == pars.end())
            break;
        indentedOutput(out, ", ");
    }
    indentedOutput(out, ")");
    return out;
}

static ostream& operator<<(ostream& out, const Function& func)
{
    indentedOutput(out, get<Function::RETURN_TYPE>(func.value_));
    indentedOutput(out, " ");
    indentedOutput(out, get<Function::NAME>(func.value_));
    indentedOutput(out, get<Function::PARAMETERS>(func.value_));
    indentedOutput(out, "\n");
    indentedOutput(out, get<Function::BODY>(func.value_));
    return out;
}

ostream& operator<<(ostream& out, const Program& program)
{
    for(list<Function>::const_iterator i = program.begin(); i != program.end(); ++i)
    {
        indentedOutput(out, *i);
        indentedOutput(out, "\n");
    }
    return out;
}


