#include "pprinter.hpp"
#include "../Packrat/pst.hpp"
#include "../Wrapper/oindentstream.hpp"
#include <iostream>
#include <tuple>

using namespace packrat;
using namespace pst;
static wrapper::oIndentStream& operator<<(wrapper::oIndentStream&, const Statement&);
static wrapper::oIndentStream& operator<<(wrapper::oIndentStream&, const Expression&);


static wrapper::oIndentStream& operator<<(wrapper::oIndentStream& out, const Type& type)
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

static wrapper::oIndentStream& operator<<(wrapper::oIndentStream& out, const BinOp& op)
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

static wrapper::oIndentStream& operator<<(wrapper::oIndentStream& out, const UnaryOp& op)
{
    switch(op.value_)
    {
    case UnaryOp::NEGATE:
        return out << " - ";
    case UnaryOp::COMPLEMENT:
        return out << " ~ ";
    case UnaryOp::NOT:
        return out << " ! ";
    case UnaryOp::REFERENCE:
        return out << " & ";
    case UnaryOp::DEREFERENCE:
        return out << " * ";
    default:
        assert(0);
    }
}


static wrapper::oIndentStream& operator<<(wrapper::oIndentStream& out, const Arguments& arguments)
{
    out << "(";
    for(Arguments::const_iterator i = arguments.begin(); i != arguments.end(); )
    {
        out << *i;
        if(++i != arguments.end())
            out << ",";
    }
    out << ")";
    return out;
}

static wrapper::oIndentStream& operator<<(wrapper::oIndentStream& out, const Call& call)
{
    return out  << get<Call::IDENTIFIER>(call.value_)
                << get<Call::ARGUMENTS>(call.value_);
}

static wrapper::oIndentStream& operator<<(wrapper::oIndentStream& out, const UnaryExpression& bexpr)
{
    return out  << "(" << get<UnaryExpression::OP>(bexpr.value_)
                << get<UnaryExpression::VALUE>(bexpr.value_) << ")";
}

static wrapper::oIndentStream& operator<<(wrapper::oIndentStream& out, const BinaryExpression& bexpr)
{
    return out  << "(" << get<BinaryExpression::LHS>(bexpr.value_)
                << get<BinaryExpression::OP >(bexpr.value_)
                << get<BinaryExpression::RHS>(bexpr.value_) << ")";
}

static wrapper::oIndentStream& operator<<(wrapper::oIndentStream& out, const Expression& expr)
{
    assert((int)expr.value_ != -1);
    switch(expr.value_)
    {
    case Expression::IDENTIFIER:
        out << expr.value_.get<Expression::IDENTIFIER>();
        break;
    case Expression::INTEGER:
        out << expr.value_.get<Expression::INTEGER>();
        break;
    case Expression::UNARY:
        out << expr.value_.get<Expression::UNARY>();
        break;
    case Expression::BINARY:
        out << expr.value_.get<Expression::BINARY>();
        break;
    case Expression::CALL:
        out << expr.value_.get<Expression::CALL>();
        break;
    default:
        assert(0);
    }
    return out;
}

static wrapper::oIndentStream& operator<<(wrapper::oIndentStream& out, const Initializer& init)
{
    return out << get<Initializer::INDENTIFIER>(init.value_)
               << " = " << get<Initializer::VALUE>(init.value_);
}

static wrapper::oIndentStream& operator<<(wrapper::oIndentStream& out, const Declaration& decl)
{
    switch(decl.value_)
    {
    case Declaration::INITIALIZER:
        out << decl.value_.get<Declaration::INITIALIZER>();
        break;
    case Declaration::DEFAULT:
        out << decl.value_.get<Declaration::DEFAULT>();
        break;
    default:
        assert(0);
    }
    return out;
}

static wrapper::oIndentStream& operator<<(wrapper::oIndentStream& out, const Declarations& decls)
{
    out << get<Declarations::TYPE>(decls.value_) << " ";
    for(std::list<Declaration>::const_iterator i = get<Declarations::DECLARATIONS>(decls.value_).begin();
            i != get<Declarations::DECLARATIONS>(decls.value_).end(); )
    {
        out << *i;
        if(++i != get<Declarations::DECLARATIONS>(decls.value_).end())
            out << ", ";
    }
    out << ";";
    return out;
}

static wrapper::oIndentStream& operator<<(wrapper::oIndentStream& out, const Block& block)
{
    out << "{" << wrapper::endl;
    ++out;
    for(Block::const_iterator i = block.begin(); i != block.end(); ++i)
        out << *i << wrapper::endl;
    --out;
    out << "}" << wrapper::endl;
    return out;
}

static wrapper::oIndentStream& operator<<(wrapper::oIndentStream& out, const Statement& statement)
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
    case Statement::DECLARATIONS:
        out << statement.value_.get<Statement::DECLARATIONS>();
        break;
    case Statement::BLOCK:
        out << statement.value_.get<Statement::BLOCK>();
        break;
    default:
        assert(0);
    }
    return out;
}

static wrapper::oIndentStream& operator<<(wrapper::oIndentStream& out, const Parameter& par)
{
    return out  << get<Parameter::TYPE>(par.value_)
                << " " <<  get<Parameter::NAME>(par.value_);
}

static wrapper::oIndentStream& operator<<(wrapper::oIndentStream& out, const Parameters& pars)
{
    Parameters::const_iterator i = pars.begin();
    out << "(";
    if(i != pars.end()) for(;;)
    {
        out << *i;
        if(++i == pars.end())
            break;
        out << ", ";
    }
    out << ")";
    return out;
}

static wrapper::oIndentStream& operator<<(wrapper::oIndentStream& out, const Function& func)
{
    out << get<Function::RETURN_TYPE>(func.value_)
        << " "
        << get<Function::NAME>(func.value_)
        << get<Function::PARAMETERS>(func.value_)
        << wrapper::endl
        << get<Function::BODY>(func.value_);
    return out;
}

ostream& operator<<(ostream& out, const Program& program)
{
    wrapper::oIndentStream wrap(out);
    for(list<Function>::const_iterator i = program.begin(); i != program.end(); ++i)
        wrap << *i << wrapper::endl;
    return out;
}


