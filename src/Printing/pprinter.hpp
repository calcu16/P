#ifndef PRINTIN_PPRINTER_HPP
#define PRINTIN_PPRINTER_HPP
#include "../Packrat/pst.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream&, const packrat::pst::Program&);

#endif
