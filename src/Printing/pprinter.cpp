#include "pprinter.hpp"
#include "../Packrat/pst.hpp"
#include <iostream>
#include <tuple>

using namespace std;
using namespace packrat;
using namespace pst;
static ostream& operator<<(ostream&, const Statement&);
static ostream& operator<<(ostream&, const Expression&);

static ostream& operator<<(ostream& out, const Type& type)
{
    assert((int)type.value_ != -1);
    switch(type.value_)
    {
    case Type::TYPENAME:
        out << type.value_.get<Type::TYPENAME>();
        break;
    default:
        assert(0);
    }
    return out;
}

static ostream& operator<<(ostream& out, const Expression& expr)
{
    assert((int)expr.value_ != -1);
    switch(expr.value_)
    {
    case Expression::IDENTIFIER:
        out << expr.value_.get<Expression::IDENTIFIER>();
        break;
    case Expression::UNARY:
        out << "un";
        break;
    case Expression::BINARY:
        out << "bin";
        break;
    default:
        assert(0);
    }
    return out;
}

static ostream& operator<<(ostream& out, const Block& block)
{
    out << "{" << endl;
    for(Block::const_iterator i = block.begin(); i != block.end(); ++i)
        out << *i << endl;
    out << "}" << endl;
    return out;
}

static ostream& operator<<(ostream& out, const Statement& statement)
{
    assert((int)statement.value_ != -1);
    switch(statement.value_)
    {
    case Statement::RETURN:
        out << "return " << statement.value_.get<Statement::RETURN>() << ";";
        break;
    case Statement::SIMPLE:
        out << statement.value_.get<Statement::SIMPLE>() << ";";
        break;
    case Statement::BLOCK:
        out << statement.value_.get<Statement::BLOCK>();
        break;
    default:
        assert(0);
    }
    return out;
}

static ostream& operator<<(ostream& out, const Parameter& par)
{
    return out  << get<Parameter::TYPE>(par.value_)
                << " "
                << get<Parameter::NAME>(par.value_);
}

static ostream& operator<<(ostream& out, const Parameters& pars)
{
    Parameters::const_iterator i = pars.begin();
    out << "(";
    if(i != pars.end()) for(;;)
    {
        out << pars;
        if(++i == pars.end())
            break;
        out << ", ";
    }
    out << ")";
    return out;
}

static ostream& operator<<(ostream& out, const Function& func)
{
    out << get<Function::RETURN_TYPE>(func.value_)
        << " "
        << get<Function::NAME>(func.value_) 
        << get<Function::PARAMETERS>(func.value_)
        << endl
        << get<Function::BODY>(func.value_);
    return out;
}

ostream& operator<<(ostream& out, const Program& program)
{
    for(list<Function>::const_iterator i = program.begin(); i != program.end(); ++i)
        out << *i << endl;
    return out;
}
