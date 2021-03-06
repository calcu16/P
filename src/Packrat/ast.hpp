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
#ifndef PACKRAT_AST_HPP
#define PACKRAT_AST_HPP
#include "../Wrapper/iterator.hpp"
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

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
        struct AST_;
        typedef std::shared_ptr<AST_> AST_t;
        typedef std::vector<AST> numbered_t;
        typedef std::unordered_map<std::string,AST> assoc_t;
        /* Wrapped class for the values */
        struct AST_
        {
            /*
             * An associative map from a string name to 
             *  an abstract syntax tree below
             * generated via <name=constant> or <name:peg>
             */
            assoc_t assoc_;
            /*
             * A list of abstract syntax trees, 
             * generated via the + or * operator
             */
            numbered_t numbered_;
            /*
             * a string value representing the matched string
             */
            std::shared_ptr<std::string> value_;
            /*
             * The start and end of this string from the original string.
             */
            int start_, end_;
            /*
             * The cost associated with making this AST
             */
            int cost_;
            
            AST_(int,int,int=0);
            AST_(int,int,const std::string&,int);
            std::ostream& print(std::ostream&, std::string) const;
        };
        AST_t tree_;
    public:
        /*
         * Typedefing both CS70 style and STL style iterators
         */
        typedef numbered_t::const_iterator ConstIterator;
        typedef numbered_t::iterator Iterator;
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
    public:
        /* A Nil AST, represents no match */
        explicit AST(int=-1);
        AST(int,int,const std::string&,int=0);
    private:
        AST(AST_t);
        AST(int,int,int=0);
        AST(const AST&,const AST&);
        AST(const std::string&, const AST&);
    public:
        /* Use default copy contructor etc. */
        void swap(AST&);
        
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
