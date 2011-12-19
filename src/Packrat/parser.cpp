#include "parser.hpp"
#include "symbol.hpp"
#include "ast.hpp"
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;
using namespace packrat;

const Parser* Parser::PParser = NULL;

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
    return add(name, Symbol(value));
}

AST Parser::lookup(const std::string& name, const std::string& input, size_t index, AST** table) const
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
