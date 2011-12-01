#ifndef PACKRAT_PARSER_HPP
#define PACKRAT_PARSER_HPP
#include <string>
#include <unordered_map>
#include <vector>
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
    };
}
#include "parser_private.hpp"
#endif
