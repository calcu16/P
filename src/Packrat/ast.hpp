#ifndef PACKRAT_AST_HPP
#define PACKRAT_AST_HPP
#include "../Wrapper/iterator.hpp"
#include "../Wrapper/constiterator.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <list>
namespace packrat
{
    class AST
    {
    private:
        static const std::string NO_STRING;
        static const AST NO_AST;
        
        typedef std::vector<AST*> numbered_t;
        std::unordered_map<std::string, AST*> assoc_;
        numbered_t numbered_;
        std::string *value_;
        
        int start_, end_;
        
    public:
        /* Match of a string */
        explicit AST(int=-1);
        AST(int,int,const std::string&);
    private:
        AST(int,int);
        AST(const AST&, const AST&);
        AST(const std::string&, const AST&);
        
    public:
        AST(const AST&);
        ~AST();
        void swap(AST&);
        AST& operator=(AST);
        
        
        /* Names a abstract syntax tree */
        AST operator() (const std::string&) const;
        
        AST operator<<(const AST&) const;
        AST& operator<<=(const AST&);
        AST operator>>(const AST&) const;
        AST& operator>>=(const AST&);
        AST operator+(const AST&) const;
        AST& operator+=(const AST&);
        
        // flattens the values
        AST flatten() const;
        
        // gets the value
        std::string& operator*();
        const std::string& operator*() const;
        
        AST& operator[](size_t);
        const AST& operator[](size_t) const;
        size_t length() const;
        
        AST& operator[](const std::string&);
        const AST& operator[](const std::string&) const;
        AST& operator[](const char*);
        const AST& operator[](const char*) const;
        size_t size() const;
        
        operator bool() const;
        bool operator!() const;
        
        int startc() const;
        int endc() const;

        typedef wrapper::ConstIterator<numbered_t,AST> ConstIterator;
        typedef wrapper::Iterator<numbered_t,AST> Iterator;
        
        typedef ConstIterator const_iterator;
        typedef Iterator iterator;
        
        
        ConstIterator begin() const;
        Iterator begin();
        
        ConstIterator end() const;
        Iterator end();
        
        std::ostream& print(std::ostream&) const;
    private:
        std::ostream& print(std::ostream&,std::string) const;

    };
}
void swap(packrat::AST&,packrat::AST&);
std::ostream& operator<<(std::ostream&,const packrat::AST&);
#endif
