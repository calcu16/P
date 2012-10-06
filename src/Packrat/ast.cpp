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

AST::AST_::AST_(int start, int end, int cost)
    : value_(nullptr), start_(start), end_(end), cost_(cost)
{
    // Nothing else to do
}

AST::AST_::AST_(int start, int end, const string& value, int cost)
    : value_(make_shared<string>(value)), start_(start), end_(end), cost_(cost)
{
    // Nothing else to do
}

AST::AST(AST_t tree)
    : tree_(tree)
{
  // Nothing else to do
}

AST::AST(int start)
    : tree_(new AST_(start,-1))
{
  // Nothing else to do
}

AST::AST(int start, int end, int cost)
    : tree_(new AST_(start, end, cost))
{
  // Nothing else to do
}

AST::AST(int start, int end, const string& value, int cost)
    : tree_(new AST_(start, end, value, cost))
{
    // Nothing else to do
}

AST::AST(const AST& left, const AST& right)
    : tree_(new AST_(left.tree_->start_, right.tree_->end_,
                     left.tree_->cost_ + right.tree_->cost_))
{
    AST fl = left.flatten(), rl = right.flatten();
    for(auto i = fl.tree_->assoc_.begin(); i != fl.tree_->assoc_.end(); ++i)
        tree_->assoc_.insert(pair<string,AST>(i->first,i->second));
    for(auto i = rl.tree_->assoc_.begin(); i != rl.tree_->assoc_.end(); ++i)
        tree_->assoc_.insert(pair<string,AST>(i->first,i->second));
    if(fl.tree_->value_ && rl.tree_->value_)
        tree_->value_ = shared_ptr<string>(new string(*fl + *rl));
}

AST::AST(const string& name, const AST& other)
    :tree_(new AST_(other.tree_->start_, other.tree_->end_, other.tree_->cost_))
{
    (*this)[name] = other;
}

void AST::swap(AST& other)
{
  std::swap(tree_, other.tree_);
}

AST AST::operator()(const std::string& name) const { return AST(name, *this); }

AST AST::operator<<(const AST& rhs) const
{
    if(!rhs)
        return *this;
    AST out(tree_->start_, rhs.tree_->end_, tree_->cost_ + rhs.tree_->cost_);
    if(size() > 0 || (tree_->value_ != NULL && *tree_->value_ != ""))
        out.tree_->numbered_.push_back(*this);
    else
        for(auto i = begin(); i != end(); ++i)
            out.tree_->numbered_.push_back(*i);
    out.tree_->numbered_.push_back(rhs.tree_);
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
    AST out(tree_->start_, rhs.tree_->end_, tree_->cost_ + rhs.tree_->cost_);
    if(!*this)
        return rhs;
    out.tree_->numbered_.push_back(*this);
    if(rhs.size() > 0||(rhs.tree_->value_ != NULL && *rhs.tree_->value_ != ""))
        out.tree_->numbered_.push_back(rhs);
    else
        for(auto i = rhs.begin(); i != rhs.end(); ++i)
            out.tree_->numbered_.push_back(*i);
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
        return AST(tree_->start_);
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
    AST acc(tree_->start_, tree_->start_, "");
    for(auto i = begin(); i != end(); ++i)
        acc = AST(acc, *i);
    acc.tree_->cost_ = tree_->cost_;
    return acc;
}


const string& AST::operator*() const
{
    return tree_->value_ == nullptr ? NO_STRING : *tree_->value_;
}

string& AST::operator*()
{
    if(tree_->value_ == nullptr)
        tree_->value_ = shared_ptr<string>(new string());
    return *tree_->value_;
}


AST& AST::operator[](size_t index)
{
    while(index >= length())
        tree_->numbered_.push_back(AST());
    return tree_->numbered_[index];
}

const AST& AST::operator[](size_t index) const
{
    return index >= length() ? NO_AST : tree_->numbered_[index];
}

AST& AST::operator[](int index)
{
    return (*this)[(size_t)index];
}

const AST& AST::operator[](int index) const
{
    return (*this)[(size_t)index];
}

size_t AST::length() const { return tree_->numbered_.size(); }

AST& AST::operator[](const std::string& key)
{
    if(tree_->assoc_.find(key) == tree_->assoc_.end())
        tree_->assoc_.insert(pair<string,AST>(key, AST()));
    return tree_->assoc_.find(key)->second;
}

const AST& AST::operator[](const std::string& key) const
{
    return tree_->assoc_.find(key) == tree_->assoc_.end()
            ? NO_AST
            : tree_->assoc_.find(key)->second;
}

AST& AST::operator[](const char* key)
{
    return (*this)[string(key)];
}

const AST& AST::operator[](const char* key) const
{
    return (*this)[string(key)];
}

size_t AST::size() const { return tree_->assoc_.size(); }

AST::operator bool() const { return tree_->end_ != -1; }
bool AST::operator!() const { return tree_->end_ == -1; }
int AST::startc() const { return tree_->start_; }
int AST::endc() const { return tree_->end_; }
int AST::cost() const { return tree_->cost_; }

AST::ConstIterator AST::begin() const
{
    return tree_->numbered_.begin();
}

AST::Iterator AST::begin()
{
    return tree_->numbered_.begin();
}

AST::ConstIterator AST::end() const
{
    return tree_->numbered_.end();
}

AST::Iterator AST::end()
{
    return tree_->numbered_.end();
}

ostream& AST::AST_::print(ostream& out, string tab) const
{
    string ntab = tab + "\t";
    out << tab << "#" << cost_ << endl;
    if(assoc_.size() > 0)
        for(auto j = assoc_.begin(); j != assoc_.end(); ++j)
            j->second.tree_->print(out << tab << j->first << endl, ntab);
    else if(numbered_.size() > 0)
    {
        size_t i = 0;
        for(auto j = numbered_.begin(); j != numbered_.end(); ++j, ++i)
            j->tree_->print(out << tab << i << endl, ntab);
    }
    else if(value_ != nullptr)
        out << tab << ": \"" << *value_ << '"' << endl;
    return out;
}

ostream& AST::print(ostream& out) const
{
    return tree_->print(out << endl << "<AST " << tree_->start_
                            << " - " << tree_->end_ << endl, "")
                << ">" << endl;
}

void swap(AST& left, AST& right) { left.swap(right); }


ostream& operator<<(ostream& out, const AST& obj)
{
    return obj.print(out);
}
