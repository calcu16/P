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
                    NAMING, PUSH_FIRST, PUSH_LAST, NOT, CONSTANT,
                    FLATTEN, COST};
        
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
        Symbol(const std::string&, const Symbol&);
        
        static char escape(char);
        static Symbol interpretString(const std::string&);
        std::ostream& print(std::ostream&,std::string) const;
        
        static int matchToInt(const AST& tree);
        static Symbol matchToSymbol(const AST& tree);
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
