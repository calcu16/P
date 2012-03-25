#ifndef PACKRAT_AST_HPP
#define PACKRAT_AST_HPP
#include "../Wrapper/iterator.hpp"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <list>

namespace packrat
{
/*
 * This class represents an (A)bstract (S)yntax (T)ree
 *
 * Most likely this class was generated by a parser,
 *  and will be turned into the syntax tree of the type
 *  of data this represents.
 *
 * AC
 */
    class AST
    {

    private:
/*
 * typedef because I keep changing this back and forth between list and array
 */
typedef std::vector<AST*> numbered_t;

/*
 * Typedefing both CS70 style and STL style iterators
 */
    public:
typedef wrapper::Iterator<numbered_t::const_iterator,const AST> ConstIterator;
typedef wrapper::Iterator<numbered_t::iterator,AST> Iterator;

typedef ConstIterator const_iterator;
typedef Iterator iterator;
    private:
        /*
         * These memebers represent the string and AST
         * that will be returned if a lookup occurs on
         * a non-existant object
         */
        static const std::string NO_STRING;
        static const AST NO_AST;
        
        /*
         * An associative map from a string name to 
         *  an abstract syntax tree below
         * generated via <name=constant> or <name:peg>
         */
        std::unordered_map<std::string, AST*> assoc_;
        /*
         * A list of abstract syntax trees, 
         * generated via the + or * operator
         */
        numbered_t numbered_;
        /*
         * a string value representing the matched string
         */
        std::string *value_;
        /*
         * The start and end of this string from the original string.
         */
        int start_, end_;
        /*
         * The cost associated with making this AST
         */
        int cost_;
    public:
        /* A Nil AST, represents no match */
        explicit AST(int=-1);
        /* Match of a string */
        AST(int,int,const std::string&,int=0);
    private:
        AST(int,int,int=0);
        AST(const AST&, const AST&);
        AST(const std::string&, const AST&);
    public:
        /*
         * This class operates on pointers
         * and therefore requires replacing
         * default copy construct/assignment/destructor
         */
        AST(const AST&);
        ~AST();
        void swap(AST&);
        AST& operator=(AST);
        
        
        /* Names a abstract syntax tree */
        AST operator() (const std::string&) const;
        
        /* 
         * appends the rhs to the lhs's list
         * turns lhs into a list if it isn't one already
         */
        AST operator<<(const AST&) const;
        AST& operator<<=(const AST&);
        /* 
         * appends the lhs to the rhs's list
         * turns rhs into a list if it isn't one already
         */
        AST operator>>(const AST&) const;
        AST& operator>>=(const AST&);
        /*
         * merges to AST's together, and flattens any lists
         */
        AST operator+(const AST&) const;
        AST& operator+=(const AST&);
        
        /*
         * Returns an AST that matches the whole string
         *  without the sublevels of the current AST
         */
        AST flatten() const;
        
        /*
         * gets the string value matched by this AST
         */
        std::string& operator*();
        const std::string& operator*() const;
        
        /*
         * looks up numerically a particular index into
         * this AST
         */
        AST& operator[](int);
        const AST& operator[](int) const;
        AST& operator[](size_t);
        const AST& operator[](size_t) const;
        size_t length() const;
        
        /*
         * Looks up by string children of this AST
         */
        AST& operator[](const std::string&);
        const AST& operator[](const std::string&) const;
        AST& operator[](const char*);
        const AST& operator[](const char*) const;
        size_t size() const;
        
        /*
         * ASTs that match a string return true, otherwise false
         */
        operator bool() const;
        bool operator!() const;
        
        /*
         * The start and end character of this match
         * with reference to the parsed string.
         */
        int startc() const;
        int endc() const;
        int cost() const;
        
        /*
         * iterator methods for the numerical indices
         */
        ConstIterator begin() const;
        Iterator begin();
        
        ConstIterator end() const;
        Iterator end();
        
        /* print methods */
        std::ostream& print(std::ostream&) const;
    private:
        std::ostream& print(std::ostream&,std::string) const;
    };
}
/*
 * non-class duplicates of internal methods
 */
void swap(packrat::AST&,packrat::AST&);
std::ostream& operator<<(std::ostream&,const packrat::AST&);
#endif
