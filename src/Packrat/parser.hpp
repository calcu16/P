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
#ifndef PACKRAT_PARSER_HPP
#define PACKRAT_PARSER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

namespace packrat
{
    class Symbol;
    class AST;
    class Parser
    {
    private:
        std::unordered_map<std::string, size_t> lookup_;
        std::vector<Symbol*> symbols_;
    public:
        Parser();
        Parser(const Parser&);
        template<typename... Args>
        Parser(Args...);
        
        static Parser create(std::ifstream&);
        static Parser create(const std::string&);
        
        ~Parser();
        Parser& swap(Parser&);
        Parser& operator=(Parser);
        
        size_t size() const;
        
        Parser& add(const std::string&, const Symbol&);
        Parser& add(const std::string&, const std::string&);
        template<typename... Args>
        Parser& add(const std::string&, const Symbol&, Args...);
        template<typename... Args>
        Parser& add(const std::string&, const std::string&, Args...);
        
        AST lookup(const std::string&, const std::string&, size_t, AST**) const;
        AST parse(const std::string&, const std::string&) const;
        static const Parser& getPParser();
    };
}
#include "parser.ipp"
#endif
