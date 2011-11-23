#ifndef PACKRAT_MATCH_HPP
#define PACKRAT_MATCH_HPP
#include <vector>
#include <unordered_map>
#include <string>
namespace packrat
{
    class Match
    {
    private:
        std::unordered_map<std::string, Match*> tree_;
        std::vector<Match*> array_;
        std::string* match_;
        int start_, end_;
        
        Match flatten() const;
        Match(const Match&, const Match&);
        Match(const std::string&, const Match&);
    public:
        Match(const Match&);
        ~Match();
        void swap(Match&);
        Match& operator=(Match);
        
        /* Match of a string */
        Match(int=-1,int=-1);
        
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
    };
}
#endif
