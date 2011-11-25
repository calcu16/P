#ifndef PACKRAT_MATCH_HPP
#define PACKRAT_MATCH_HPP
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
namespace packrat
{
    class Match
    {
    private:
        std::unordered_map<std::string, Match*> tree_;
        std::vector<Match*> array_;
        std::string* match_;
        int start_, end_;
        
        Match(const Match&, const Match&);
        Match(const std::string&, const Match&);
        std::ostream& print(std::ostream&,std::string) const;
    public:
        Match(const Match&);
        ~Match();
        void swap(Match&);
        Match& operator=(Match);
        
        /* Match of a string */
        Match(int=-1,int=-1);
        Match(int,int,const std::string&);
        
        /* names a match, extending the parse tree */
        Match operator() (const std::string&) const;
        /* Adds the match to the end of the list */
        Match operator<<(const Match&) const;
        /* Adds this match to the beginning of the other list */
        Match operator>>(const Match&) const;
        /* Appends the match to this match
         * if either match is not an array match it
         * turns it into a 1 element array */
        Match operator+(const Match&) const;
        /* flattens both matches and combines them */
        Match operator&(const Match&) const;
        /* flattens an array match */
        Match operator&() const;
        
        /* lookup match in array */
        Match& operator[](size_t);
        Match operator[](size_t) const;
        /* lookup match in tree */
        Match& operator[](const std::string&);
        Match operator[](const std::string&) const;
        
        operator bool() const;
        bool operator!() const;
        
        int start() const;
        int end() const;
        
        int length() const;
        int size() const;
        
        Match flatten() const;
        
        std::ostream& print(std::ostream&) const;
    };
}

std::ostream& operator<<(std::ostream&,const packrat::Match&);
#endif
