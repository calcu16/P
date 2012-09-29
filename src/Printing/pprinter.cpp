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
#include "pprinter.hpp"
#include "../Packrat/pst.hpp"
#include "../Wrapper/oindentstream.hpp"
#include <string>
#include <iostream>
#include <list>
#include <tuple>

using namespace packrat;
using namespace pst;

static const string* CURRENT_TYPE = NULL;
static bool REFERENCE = false;

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream&, const Statement&);

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream&, const Expression&);

/*
static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const Type& type)
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
*/
static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const BinOp& op)
{
    switch(op.value_)
    {
    case BinOp::COMMA:
        return out << " , ";
    case BinOp::ASSIGN:
        return out << " = ";
    case BinOp::PLUS:
        return out << " + ";
    case BinOp::MINUS:
        return out << " - ";
    case BinOp::TIMES:
        return out << " * ";
    case BinOp::DIVIDE:
        return out << " / ";
    case BinOp::EQ:
        return out << " == ";
    case BinOp::NEQ:
        return out << " != ";
    case BinOp::GT:
        return out << " > ";
    case BinOp::LT:
        return out << " < ";
    case BinOp::GEQ:
        return out << " >= ";
    case BinOp::LEQ:
        return out << " <= ";
    default:
        assert(0);
    }
}

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const UnaryOp& op)
{
    switch(op.value_)
    {
    case UnaryOp::NEGATE:
        return out << "-";
    case UnaryOp::COMPLEMENT:
        return out << "~";
    case UnaryOp::NOT:
        return out << "!";
    case UnaryOp::REFERENCE:
        return out << "&";
    case UnaryOp::DEREFERENCE:
        return out << "*";
    default:
        assert(0);
    }
}


static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const Arguments& arguments)
{
    out << "(";
    for(Arguments::const_iterator i = arguments.begin(); i != arguments.end(); )
    {
        *out = 1;
        out << *i;
        if(++i != arguments.end())
            out << ",";
    }
    out << ")";
    return out;
}

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const Call& call)
{
    return out  << get<Call::IDENTIFIER>(call.value_)
                << get<Call::ARGUMENTS>(call.value_);
}

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const UnaryExpression& uexpr)
{
    if(CURRENT_TYPE == NULL)
    {
        int nprec = UnaryOp::prec,
            cprec = *out;
        if(nprec < cprec)
            out << "(";
        *out = nprec;
        out << get<UnaryExpression::OP>(uexpr.value_)
            << get<UnaryExpression::VALUE>(uexpr.value_);
        if(nprec < cprec)
            out << ")";
        return out;
    } else {
        assert(get<UnaryExpression::OP>(uexpr.value_).value_
            == UnaryOp::DEREFERENCE);
        string builder = "FatPointer<" + *CURRENT_TYPE + " >";
        const string* last = CURRENT_TYPE;
        CURRENT_TYPE = new string(builder);
        out << get<UnaryExpression::VALUE>(uexpr.value_);
        delete CURRENT_TYPE;
        CURRENT_TYPE = last;
    }
    return out;
}

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const BinaryExpression& bexpr)
{
    int op    = get<BinaryExpression::OP >(bexpr.value_).value_,
        nprec = BinOp::prec[op],
        cprec = *out;
    // std::cerr << nprec << " vs. " << cprec;
    if(nprec < cprec)
        out << "(";
    *out = nprec + (BinOp::assoc[op] != BinOp::L);
    // std::cerr << "*" << *out << std::endl;
    out << get<BinaryExpression::LHS>(bexpr.value_);
    out << get<BinaryExpression::OP >(bexpr.value_);
    *out = nprec + (BinOp::assoc[op] != BinOp::R);
    // std::cerr << "*" << *out << std::endl;
    out << get<BinaryExpression::RHS>(bexpr.value_);
    if(nprec < cprec)
        out << ")";
    return out;
}

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const Index& index)
{
    *out = Index::prec;
    out  << get<Index::VALUE>(index.value_);
    *out = 0;
    out<< "[" << get<Index::INDEX>(index.value_) << "]";
    return out;
}

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const Expression& expr)
{
    assert((int)expr.value_ != -1);
    switch(expr.value_)
    {
    case Expression::IDENTIFIER:
        if(CURRENT_TYPE != NULL)
            out << *CURRENT_TYPE << (REFERENCE ? "& " : " ")
                << expr.value_.get<Expression::IDENTIFIER>();
        else
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
    case Expression::INDEX:
        out << expr.value_.get<Expression::INDEX>();
        break;
    default:
        assert(0);
    }
    return out;
}

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const Initializer& init)
{
    const string* last = CURRENT_TYPE;
    CURRENT_TYPE = new string(*last);
    out << get<Initializer::INDENTIFIER>(init.value_);
    delete CURRENT_TYPE;
    CURRENT_TYPE = NULL;
    *out = 1;
    out << " = " << get<Initializer::VALUE>(init.value_);
    CURRENT_TYPE = last;
    return out;
}

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const Declaration& decl)
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

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const Declarations& decls)
{
    CURRENT_TYPE = &get<Declarations::TYPE>(decls.value_)
                        .value_.get<Type::TYPENAME>();
    // out << get<Declarations::TYPE>(decls.value_) << " ";
    for(std::list<Declaration>::const_iterator i
            = get<Declarations::DECLARATIONS>(decls.value_).begin();
            i != get<Declarations::DECLARATIONS>(decls.value_).end(); )
    {
        out << *i;
        if(++i != get<Declarations::DECLARATIONS>(decls.value_).end())
            out << ";" << wrapper::endl;
        //    out << ", ";
    }
    CURRENT_TYPE = NULL;
    // out << ";";
    return out;
}

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const Block& block)
{
    out << "{" << wrapper::endl;
    ++out;
    for(Block::const_iterator i = block.begin(); i != block.end(); ++i)
        out << *i << wrapper::endl;
    --out;
    out << "}";
    return out;
}

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const ForLoop& forLoop)
{
    out << "for("   << get<ForLoop::INIT>(forLoop.value_);
    *out = 0;
    out << "; "     << get<ForLoop::COND>(forLoop.value_);
    *out = 0;
    out << "; "     << get<ForLoop::INC >(forLoop.value_);
    out << ")"      << wrapper::endl;
    out << (Block)get<ForLoop::BODY>(forLoop.value_);
    return out;
}

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const If& ifs)
{
    out << "if("   << get<If::COND>(ifs.value_);
    out << ")"      << wrapper::endl;
    out << (Block)get<If::BODY>(ifs.value_);
    return out;
}

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const Statement& statement)
{
    assert((int)statement.value_ != -1);
    *out = 0;
    switch(statement.value_)
    {
    case Statement::RETURN:
        out << "return " << statement.value_.get<Statement::RETURN>() << ";";
        break;
    case Statement::SIMPLE:
        out << statement.value_.get<Statement::SIMPLE>() << ";";
        break;
    case Statement::DECLARATIONS:
        out << statement.value_.get<Statement::DECLARATIONS>() << ";";
        break;
    case Statement::FOR:
        out << statement.value_.get<Statement::FOR>();
        break;
    case Statement::IF:
        out << statement.value_.get<Statement::IF>();
        break;
    case Statement::BLOCK:
        out << statement.value_.get<Statement::BLOCK>();
        break;
    default:
        assert(0);
    }
    return out;
}

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const Parameter& par)
{
    // if(get<Parameter::CONST>(par.value_))
    //    out << "const ";
    REFERENCE = !get<Parameter::CONST>(par.value_);
    CURRENT_TYPE = &get<Parameter::TYPE>(par.value_)
                        .value_.get<Type::TYPENAME>();
    out << get<Parameter::NAME>(par.value_);
    REFERENCE = false;
    CURRENT_TYPE = NULL;
    return out;
    // return out  << get<Parameter::TYPE>(par.value_)
    //            << " " <<  get<Parameter::NAME>(par.value_);
}

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const Parameters& pars)
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

static wrapper::oIndentStream&
operator<<(wrapper::oIndentStream& out, const Function& func)
{
    REFERENCE = false;
    CURRENT_TYPE = &get<Function::RETURN_TYPE>(func.value_)
                            .value_.get<Type::TYPENAME>();
    out << get<Function::NAME>(func.value_);
    CURRENT_TYPE = NULL;
    /*
    out << get<Function::RETURN_TYPE>(func.value_)
        << " "
        << get<Function::NAME>(func.value_);
    */
    out << get<Function::PARAMETERS>(func.value_)
        << wrapper::endl
        << get<Function::BODY>(func.value_);
    return out;
}

ostream&
operator<<(ostream& out, const Program& program)
{
    CURRENT_TYPE = NULL;
    wrapper::oIndentStream wrap(out);
    for(list<Function>::const_iterator i = program.begin();
            i != program.end(); ++i)
        wrap << *i << wrapper::endl;
    return out;
}


