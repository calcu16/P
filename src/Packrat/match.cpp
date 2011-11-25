#include "match.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;
using namespace packrat;

Match::Match(int start, int end)
    : match_(new string()), start_(start), end_(end)
{
}

Match::Match(int start, int end, const string& m)
    : match_(new string(m)), start_(start), end_(end)
{
}

Match::Match(const Match& copy)
    : match_(copy.match_ == NULL ? NULL : new string(*copy.match_)),
        start_(copy.start_), end_(copy.end_)
{
    for(unordered_map<string, Match*>::const_iterator i = copy.tree_.begin(); i != copy.tree_.end(); ++i)
        tree_.insert(pair<string, Match*>(i->first, new Match(*i->second)));
    for(vector<Match*>::const_iterator i = copy.array_.begin(); i != copy.array_.end(); ++i)
        array_.push_back(new Match(**i));
}

Match::Match(const Match& left, const Match& right)
    : match_(NULL), start_(left.start_), end_(right.end_)
{
    Match fl = left.flatten(), rl = right.flatten();
    for(unordered_map<string, Match*>::iterator i = fl.tree_.begin(); i != fl.tree_.end(); ++i)
        tree_.insert(pair<string, Match*>(i->first, new Match(*i->second)));
    for(unordered_map<string, Match*>::iterator i = rl.tree_.begin(); i != rl.tree_.end(); ++i)
        tree_.insert(pair<string, Match*>(i->first, new Match(*i->second)));
    if(tree_.size() == 0 && left.match_ && right.match_)
        match_ = new string(*left.match_ + *right.match_);
}

Match::Match(const std::string& name, const Match& other)
    : match_(NULL), start_(other.start_), end_(other.end_)
{
    tree_[name] = new Match(other);
}

Match::~Match()
{
    for(unordered_map<string, Match*>::iterator i = tree_.begin(); i != tree_.end(); ++i)
        delete i->second;
    for(vector<Match*>::iterator i = array_.begin(); i != array_.end(); ++i)
        delete *i;
}

void Match::swap(Match& rhs)
{
    std::swap(match_, rhs.match_);
    std::swap(tree_, rhs.tree_);
    std::swap(array_, rhs.array_);
    std::swap(start_, rhs.start_);
    std::swap(end_, rhs.end_);
}

Match Match::flatten() const
{
    if(array_.size() == 0)
        return *this;
    Match acc(start_, end_);
    for(vector<Match*>::const_iterator i = array_.begin(); i != array_.end(); ++i)
        acc = Match(acc, **i);
    return acc;
}

Match& Match::operator=(Match rhs)
{
    swap(rhs);
    return *this;
}

Match Match::operator() (const string& name) const
{
    if(*this)
        return Match(name, *this);
    return *this;
}

Match Match::operator<<(const Match& m) const
{
    if(tree_.size() > 0)
    {
        Match out(start_, m.end_);
        out.array_.push_back(new Match(*this));
        out.array_.push_back(new Match(m));
        return out;
    }
    else
    {
        Match out(*this);
        out.end_ = m.end_;
        out.array_.push_back(new Match(m));
        return out;
    }
}

Match Match::operator>>(const Match& m) const
{
    Match out(start_, m.end_);
    if(tree_.size() > 0)
    {
        out.array_.push_back(new Match(*this));
        out.array_.push_back(new Match(m));
        return out;
    }
    else
    {
        out.array_.push_back(new Match(*this));
        for(vector<Match*>::const_iterator i = m.array_.begin(); i != m.array_.end(); ++i)
            out.array_.push_back(new Match(**i));
        return out;
    }
}

Match Match::operator+(const Match& m) const
{
    Match out(this->start_, m.end_);
    for(vector<Match*>::const_iterator i = array_.begin(); i != array_.end(); ++i)
        out.array_.push_back(new Match(**i));
    if(tree_.size())
        out.array_.push_back(new Match(*this));
    for(vector<Match*>::const_iterator i = m.array_.begin(); i != m.array_.end(); ++i)
        out.array_.push_back(new Match(**i));
    if(tree_.size())
        out.array_.push_back(new Match(m));
    return out;
}

Match Match::operator&() const
{
    return flatten();
}

Match Match::operator&(const Match& m) const
{
    if(!*this)
        return *this;
    if(!m)
        return Match(start_, -1);
    return Match(*this, m);
}

Match& Match::operator[](size_t index)
{
    return *array_[index];
}

Match Match::operator[](size_t index) const
{
    return *array_[index];
}

Match& Match::operator[](const string& index)
{
    return *tree_.find(index)->second;
}

Match Match::operator[](const string& index) const
{
    return *tree_.find(index)->second;
}

Match::operator bool() const
{
    return end_ != -1;
}

bool Match::operator!() const
{
    return end_ == -1;
}

int Match::start() const
{
    return start_;
}

int Match::end() const
{
    return end_;
}

int Match::size() const
{
    return tree_.size();
}

int Match::length() const
{
    return array_.size();
}

ostream& Match::print(ostream& out, string tab) const
{
    string ntab = tab + "\t";
    size_t i = 0;
    if(size())
        for(unordered_map<string, Match*>::const_iterator i = tree_.begin(); i != tree_.end(); i++)
            i->second->print(out << tab << i->first << endl, ntab);
    else if(length())
        for(vector<Match*>::const_iterator j = array_.begin(); j != array_.end(); j++, i++)
            (*j)->print(out << tab << i << endl, ntab);
    else
    {
        out << tab << ": \"" << *match_ << '"' << endl;
    }
    return out;
}

ostream& Match::print(ostream& out) const
{
    return print(out, "");
}

ostream& operator<<(ostream& out, const Match& m)
{
    return m.print(out);
}
