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
#ifndef PACKRAT_SYMBOL_HPP
#define PACKRAT_SYMBOL_HPP
#include <string>
namespace packrat
{
    class Parser;
    class AST;
    
    class Symbol
    {
    private:
        static void createInterpreter();
        
        enum Type { NONE, MATCH, SET, RANGE, NEXT,
                    LOOKUP, REPEAT, CONCAT, EITHER,
                    NAMING, PUSH_FIRST, PUSH_LAST, NOT, TRY,
                    CONSTANT, FLATTEN, COST};
        
        Type type_;
        std::string *match_;
        int count_;
        Symbol *left_, *right_;
        
        Symbol();
        Symbol(Type, const std::string&);
        Symbol(int);
        Symbol(Type, int);
        Symbol(int , const Symbol&);
        Symbol(Type, const Symbol&);
        Symbol(Type, const Symbol&, const Symbol&);
        Symbol(Type, const Symbol&, const Symbol&, int);
        Symbol(const std::string&, const Symbol&);
        
        static char escape(char);
        static Symbol interpretString(const std::string&);
        std::ostream& print(std::ostream&,std::string) const;
        
        static int matchToInt(const AST& tree);
        static Symbol matchToSymbol(const AST& tree);
        static Symbol matchToConcatSymbol(const AST&, const AST&);
    public:
        Symbol(const Symbol&);
        Symbol(const std::string&);
        ~Symbol();
        
        static Symbol createMatch(const std::string&);
        static Symbol createMatch(char);
        static Symbol constant(const std::string&);
        static Symbol constant(const std::string&, const std::string&);
        static Symbol createSet(const std::string&);
        static Symbol createSet(char, char);
        static Symbol createNext(int=1);
        static Symbol createCost(int=1);
        static Symbol createLookup(const std::string&);
        Symbol flatten() const;
        AST match(const Parser&, const std::string&, size_t, AST**) const;
        
        void swap(Symbol&);
        
        Symbol& operator= (Symbol rhs);
        Symbol operator& (const Symbol&) const;
        Symbol operator| (const Symbol&) const;
        Symbol operator<< (const Symbol&) const;
        Symbol operator>> (const Symbol&) const;
        Symbol operator! () const;
        Symbol operator^ (int) const;
        
        Symbol operator() (const std::string&) const;
        std::ostream& print(std::ostream&) const;
    private:
        static const Parser *interpreter;
    };
}
std::ostream& operator<<(std::ostream&,const packrat::Symbol&);
void swap(packrat::Symbol&,packrat::Symbol&);

#endif
