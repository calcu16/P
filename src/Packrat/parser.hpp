#ifndef PACKRAT_PARSER_HPP
#define PACKRAT_PARSER_HPP
#include <string>
#include <unordered_map>
#include <vector>
namespace packrat
{
    class Symbol;
    class Match;
    class Parser
    {
    private:
        std::unordered_map<std::string, size_t> lookup_;
        std::vector<Symbol*> symbols_;
    public:
        Parser();
        Parser(const Parser&);
        ~Parser();
        Parser& swap(Parser&);
        Parser& operator=(Parser);
        size_t addSymbol(const std::string&, const Symbol&);
        Match lookup(const std::string&, const std::string&, size_t, Match**) const;
        
        Match parse(const std::string&, const std::string&) const;
    };
}
#endif
