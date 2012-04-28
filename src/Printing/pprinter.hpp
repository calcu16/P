#ifndef PRINTIN_PPRINTER_HPP
#define PRINTIN_PPRINTER_HPP 1
#include "../Packrat/pst.hpp"
#include <iostream>
using namespace std;

std::ostream& operator<<(std::ostream&, const packrat::pst::Program&);

extern bool IS_NEWLINE;
extern int INDENT_NO;

template<typename T>
static void indentedOutput(std::ostream &out, const T& obj)
{
    std::ostringstream temp;
    temp << obj;
    std::string message = temp.str();
    std::string::iterator cur = message.begin();

    // You will probably want to fix the rest of this.
    while (cur != message.end()) {
        if (IS_NEWLINE) {
            for (int i = 0; i < INDENT_NO; ++i) {
                out << "    ";
            }
            IS_NEWLINE = false;
        }
        out << *cur;
        if (*cur == '\n') {
            IS_NEWLINE = true;
        }
        ++cur;
    }
}


#endif
