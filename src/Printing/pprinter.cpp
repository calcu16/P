#include "pprinter.hpp"
#include "../Packrat/pst.hpp"
#include <iostream>
#include <tuple>

using namespace std;
using namespace packrat;
using namespace pst;

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

static ostream& operator<<(ostream& out, const Function& func)
{
    out << get<Function::RETURN_TYPE>(func.value_) << " "
        << get<Function::NAME>(func.value_) << endl;
    return out;
}

ostream& operator<<(ostream& out, const Program& program)
{
    for(list<Function>::const_iterator i = program.begin(); i != program.end(); ++i)
        out << *i << endl;
    return out;
}
