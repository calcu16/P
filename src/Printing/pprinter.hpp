#ifndef PRINTIN_PPRINTER_HPP
#define PRINTIN_PPRINTER_HPP 1
#include "../Packrat/pst.hpp"
#include <iostream>
using namespace std;

std::ostream& operator<<(std::ostream&, const packrat::pst::Program&);
#endif
