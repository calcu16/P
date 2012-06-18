/*
Copyright (c) 2012, Andrew Carter, Dietrich Lagenbach, Xanda Schofield
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
#include "parser.hpp"
#include "symbol.hpp"
#include "ast.hpp"
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
using namespace packrat;

Parser::Parser()
{
}

Parser::Parser(const Parser& copy)
    : lookup_(copy.lookup_)
{
    for(size_t i = 0; i < copy.symbols_.size(); i++)
        symbols_[i] = new Symbol(*copy.symbols_[i]);
}

Parser::~Parser()
{
    for(size_t i = 0; i < symbols_.size(); i++)
        delete symbols_[i];
}



Parser& Parser::swap(Parser& other)
{
    std::swap(lookup_, other.lookup_);
    std::swap(symbols_, other.symbols_);
    return *this;
}

Parser& Parser::operator=(Parser rhs)
{
    swap(rhs);
    return *this;
}


size_t Parser::size() const
{
    return symbols_.size();
}

#include <iostream>
Parser& Parser::add(const std::string& name, const Symbol& value)
{
    size_t index;
    if(lookup_.find(name) == lookup_.end())
    {
        lookup_[name] = symbols_.size();
        symbols_.push_back(new Symbol(value));
        return *this;
    }
    index = lookup_[name];
    delete symbols_[index];
    symbols_[index] = new Symbol(value);
    return *this;
}

Parser& Parser::add(const std::string& name, const std::string& value)
{
    // std::cout << "Adding " << name << std::endl;
    return add(name, Symbol(value));
}

#include <cassert>
AST Parser::lookup(const std::string& name,
                    const std::string& input, size_t index, AST** table) const
{
    size_t i;
    if(lookup_.find(name) == lookup_.end())
        return AST(index);
    i = lookup_.find(name)->second;
    if(table[i][index].startc() == -1)
    {
        table[i][index] = AST(index);
        table[i][index] = symbols_[i]->match(*this, input, index, table);
    }
    return table[i][index];
}

AST Parser::parse(const std::string& name, const std::string& input) const
{
    size_t i,j;
    AST ret, **table = new AST*[symbols_.size()];
    for(i = 0; i < symbols_.size(); ++i)
        table[i] = new AST[input.length() + 1];
    
    for(i = 0; i < symbols_.size(); ++i)
    for(j = 0; j <= input.length(); ++j)
        table[i][j] = AST(-1);
    ret = lookup(name, input, 0, table);
    delete[] table;
    return ret;
}

#include "parser_p.cpp"
