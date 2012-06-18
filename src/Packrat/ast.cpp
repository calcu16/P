/*
Copyright (c) 2012, Andrew Carter, Dietrich Lagenbach, Xanda Schofield
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
#include "ast.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include <string>
using namespace std;
using namespace packrat;

const string AST::NO_STRING = "";
const AST AST::NO_AST;

template<typename T>
static T* copyp(T* t)
{
    return t == NULL ? (T*)NULL : new T(*t);
}

AST::AST(int start)
    : value_(NULL), start_(start), end_(-1), cost_(0)
{
    // Nothing else to do
}

AST::AST(int start, int end, int cost)
    : value_(NULL), start_(start), end_(end), cost_(cost)
{
    // Nothing else to do
}

AST::AST(int start, int end, const string& value, int cost)
    : value_(new string(value)), start_(start), end_(end), cost_(cost)
{
    // Nothing else to do
}

AST::AST(const AST& left, const AST& right)
    : value_(NULL), start_(left.start_), end_(right.end_),
        cost_(left.cost_ + right.cost_)
{
    AST fl = left.flatten(), rl = right.flatten();
    for(unordered_map<string,AST*>::iterator i = fl.assoc_.begin();
                i != fl.assoc_.end(); ++i)
        assoc_.insert(pair<string,AST*>(i->first,copyp(i->second)));
    for(unordered_map<string,AST*>::iterator i = rl.assoc_.begin();
                i != rl.assoc_.end(); ++i)
        assoc_.insert(pair<string,AST*>(i->first,copyp(i->second)));
    if(fl.value_ && rl.value_)
        value_ = new string(*fl + *rl);
}

AST::AST(const string& name, const AST& other)
    : assoc_(NULL), numbered_(NULL), value_(NULL),
        start_(other.start_), end_(other.end_), cost_(other.cost_)
{
    (*this)[name] = other;
}

AST::AST(const AST& copy)
    : value_(copyp(copy.value_)), start_(copy.start_), end_(copy.end_),
        cost_(copy.cost_)
{
    for(unordered_map<string, AST*>::const_iterator i = copy.assoc_.begin();
            i != copy.assoc_.end(); ++i)
        assoc_.insert(pair<string,AST*>(i->first,copyp(i->second)));
    for(AST::const_iterator i = copy.begin(); i != copy.end(); ++i)
        numbered_.push_back(new AST(*i));
}

AST::~AST()
{
    for(unordered_map<string, AST*>::iterator i = assoc_.begin();
            i != assoc_.end(); ++i)
        delete i->second;
    for(numbered_t::iterator i = numbered_.begin(); i != numbered_.end(); ++i)
        delete *i;
    delete value_;
}

void AST::swap(AST& other)
{
    std::swap(assoc_, other.assoc_);
    std::swap(numbered_, other.numbered_);
    std::swap(value_, other.value_);
    std::swap(start_, other.start_);
    std::swap(end_, other.end_);
    std::swap(cost_, other.cost_);
}

AST& AST::operator=(AST rhs)
{
    swap(rhs);
    return *this;
}

AST AST::operator()(const std::string& name) const { return AST(name, *this); }

AST AST::operator<<(const AST& rhs) const
{
    if(!rhs)
        return *this;
    AST out(start_, rhs.end_, cost_ + rhs.cost_);
    if(size() > 0 || (value_ != NULL && *value_ != ""))
        out.numbered_.push_back(new AST(*this));
    else
        for(AST::const_iterator i = begin(); i != end(); ++i)
            out.numbered_.push_back(new AST(*i));
    out.numbered_.push_back(new AST(rhs));
    return out;
}

AST& AST::operator<<=(const AST& rhs)
{
    AST out(*this << rhs);
    swap(out);
    return *this;
}

AST AST::operator>>(const AST& rhs) const
{
    if(!*this)
        return rhs;
    AST out(start_, rhs.end_, cost_ + rhs.cost_);
    out.numbered_.push_back(new AST(*this));
    if(rhs.size() > 0 || (rhs.value_ != NULL && *rhs.value_ != ""))
        out.numbered_.push_back(new AST(rhs));
    else
        for(AST::const_iterator i = rhs.begin(); i != rhs.end(); ++i)
            out.numbered_.push_back(new AST(*i));
    return out;
}

AST& AST::operator>>=(const AST& rhs)
{
    AST out(*this >> rhs);
    swap(out);
    return *this;
}

AST AST::operator+(const AST& rhs) const
{
    if(!(*this && rhs))
        return AST(start_);
    return AST(*this, rhs);
}


AST& AST::operator+=(const AST& rhs)
{
    AST out(*this + rhs);
    swap(out);
    return *this;
}

AST AST::flatten() const
{
    if(length() == 0)
        return *this;
    AST acc(start_, start_, "");
    for(AST::const_iterator i = begin(); i != end(); ++i)
        acc = AST(acc, *i);
    acc.cost_ = cost_;
    return acc;
}


const string& AST::operator*() const
{
    return value_ == NULL ? NO_STRING : *value_;
}

string& AST::operator*()
{
    if(value_ == NULL)
        value_ = new string();
    return *value_;
}


AST& AST::operator[](size_t index)
{
    while(index >= length())
        numbered_.push_back(new AST());
    return *numbered_[index];
}

const AST& AST::operator[](size_t index) const
{
    return index >= length() ? NO_AST : *numbered_[index];
}

AST& AST::operator[](int index)
{
    return (*this)[(size_t)index];
}

const AST& AST::operator[](int index) const
{
    return (*this)[(size_t)index];
}

size_t AST::length() const { return numbered_.size(); }

AST& AST::operator[](const std::string& key)
{
    if(assoc_.find(key) == assoc_.end())
        assoc_.insert(pair<string,AST*>(key, new AST()));
    return *assoc_.find(key)->second;
}

const AST& AST::operator[](const std::string& key) const
{
    return assoc_.find(key) == assoc_.end()
            ? NO_AST
            : *assoc_.find(key)->second;
}

AST& AST::operator[](const char* key)
{
    return (*this)[string(key)];
}

const AST& AST::operator[](const char* key) const
{
    return (*this)[string(key)];
}

size_t AST::size() const { return assoc_.size(); }

AST::operator bool() const { return end_ != -1; }
bool AST::operator!() const { return end_ == -1; }
int AST::startc() const { return start_; }
int AST::endc() const { return end_; }
int AST::cost() const { return cost_; }

AST::ConstIterator AST::begin() const
{
    return ConstIterator(numbered_.begin());
}

AST::Iterator AST::begin()
{
    return Iterator(numbered_.begin());
}

AST::ConstIterator AST::end() const
{
    return ConstIterator(numbered_.end());
}

AST::Iterator AST::end()
{
    return Iterator(numbered_.end());
}

ostream& AST::print(ostream& out, string tab) const
{
    string ntab = tab + "\t";
    out << tab << "#" << cost_ << endl;
    if(size())
        for(unordered_map<string, AST*>::const_iterator j = assoc_.begin();
                    j != assoc_.end(); ++j)
            j->second->print(out << tab << j->first << endl, ntab);
    else if(length())
    {
        size_t i = 0;
        for(AST::const_iterator j = begin(); j != end(); ++j, ++i)
            j->print(out << tab << i << endl, ntab);
    }
    else if(value_ != NULL)
        out << tab << ": \"" << *value_ << '"' << endl;
    return out;
}

ostream& AST::print(ostream& out) const
{
    return print(out << endl << "<AST " << start_ << " - " << end_ << endl, "")
                << ">" << endl;
}

void swap(AST& left, AST& right) { left.swap(right); }


ostream& operator<<(ostream& out, const AST& obj)
{
    return obj.print(out);
}
