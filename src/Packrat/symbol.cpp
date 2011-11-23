#include "symbol.hpp"
#include "parser.hpp"
#include "match.hpp"
#include <string>
#include <stddef.h>
#include <iostream>

using namespace packrat;
using namespace std;

Symbol::Symbol()
    : type_(NEXT), match_ (NULL), count_(0),
        left_(NULL), right_(NULL)
{
}

Symbol::Symbol(Type t, const string& m)
    : type_ (t), match_(new string(m)),
        left_(NULL), right_(NULL)
{
}

Symbol::Symbol(int count)
    : type_ (NEXT), match_ (NULL), count_(count),
        left_(NULL), right_(NULL)
{
}

Symbol::Symbol(Type t, const Symbol& left)
    : type_ (t), match_(NULL),
        left_(new Symbol(left)), right_(NULL)
{
}

Symbol::Symbol(Type t, const Symbol& left, const Symbol& right)
    : type_ (t), match_(NULL),
        left_(new Symbol(left)), right_(new Symbol(right))
{
}

Symbol::Symbol(int a, const Symbol& left)
    : type_(REPEAT), match_(NULL), count_(a),
        left_(new Symbol(left)), right_(NULL)
{
}

Symbol::Symbol(const string& m, const Symbol& left)
    : type_ (NAMING), match_(new string(m)),
        left_(new Symbol(left)), right_(NULL)
{
}


Symbol::Symbol(const Symbol& copy)
    : type_(copy.type_), count_(copy.count_)
{
    match_ = copy.match_ ? new string(*copy.match_) : NULL;
    left_ = copy.left_ ? new Symbol(*copy.left_) : NULL;
    right_ = copy.right_ ? new Symbol(*copy.right_) : NULL;
}

char Symbol::escape(char c)
{
    switch(c)
    {
    case 'n': return '\n';
    case 'r': return '\r';
    case 't': return '\t';
    default: return c;
    }
}

Symbol Symbol::interpretString(const std::string& input, size_t& index, char term)
{
    Symbol acc = createMatch(""), mul = createSet(""), last = createMatch("");
    string match = "";
    char negate = 0;
    for(; index < input.length(); ++index)
    {
        switch(input[index])
        {
        case '!':
            if(negate)
                negate ^= 1;
            else
                negate = 1;
            break;
        case '\\':
            acc = acc & last;
            if(++index < input.length())
                last = createMatch(escape(input[index]));
            if(negate)
            {
                last = !last;
                if(!(negate & 1))
                    last = !last;
            }
            negate = 0;
            break;
        case '[':
            acc = acc & last;
            match = "";
            if(++index < input.length())
            {
                switch(input[index])
                {
                case ']':
                case '-':
                    match += input[index++];
                    break;
                default: ;
                }
            }
            for(; index < input.length(); ++index)
            {
                switch(input[index])
                {
                case '-':
                    if(++index < input.length())
                        for(char c = match[match.length()-1]; c++ < input[index]; )
                            match += c;
                    break;
                case ']':
                    goto exit_main;
                case '\\':
                    if(++index < input.length())
                        match += escape(input[index]);
                    break;
                default:
                    match += input[index];
                }
            }
            exit_main:
            last = createSet(match);
            if(negate)
            {
                last = !last;
                if(!(negate & 1))
                    last = !last;
            }
            negate = 0;
            match = "";
            break;
        case '(':
            acc = acc & last;
            last = interpretString(input, ++index, ')');
            if(negate)
            {
                last = !last;
                if(!(negate & 1))
                    last = !last;
            }
            negate = 0;
            match = "";
            break;
        case ')':
        case '>':
            if(input[index] == term)
            {
                if(negate)
                {
                    acc = acc & last;
                    last = !createNext(1);
                    if(!(negate & 1))
                        last = !last;
                }
                return mul | (acc & last);
            }
            acc = acc & last;
            last = createMatch(input[index]);
            if(negate)
            {
                last = !last;
                if(!(negate & 1))
                    last = !last;
            }
            negate = 0;
            break;
        case '{':
            acc = acc & last;
            match = "";
            for(++index; index < input.length(); ++index)
            {
                switch(input[index])
                {
                case '}':
                    goto exit_main_2;
                case '\\':
                    if(++index < input.length())
                        match += escape(input[index]);
                default:
                    match += input[index];
                }
            }
            exit_main_2:
            last = createLookup(match);
            if(negate)
            {
                last = !last;
                if(!(negate & 1))
                    last = !last;
            }
            negate = 0;
            match = "";
            break;
        case '<':
            acc = acc & last;
            for(++index; index < input.length(); ++index)
            {
                switch(input[index])
                {
                case '=':
                case ':':
                case '>':
                    goto exit_main_3;
                case '\\':
                    if(++index < input.length())
                        match += escape(input[index]);
                    break;
                default:
                    match += input[index];
                }
            }
            exit_main_3:
            if(input[index] == '>')
            {
                last = createMatch("")(match);
                ++index;
            }
            else if(input[index] == '=')
            {
                string value = "";
                for(++index; index < input.length(); ++index)
                {
                    switch(input[index])
                    {
                    case '>':
                        goto exit_main_4;
                    case '\\':
                        if(++index < input.length())
                            value += escape(input[index]);
                        break;
                    default:
                        value += input[index];
                    }
                }
                exit_main_4:
                last = constant(match, value);
            }
            else
                last = interpretString(input, ++index, '>')(match);
            if(negate)
            {
                last = !last;
                if(!(negate & 1))
                    last = !last;
            }
            negate = 0;
            match = "";
            break;
        case '*':
            if(negate)
            {
                acc = acc & last;
                if(negate & 1)
                    last = !createNext(1);
                else
                    last = !!createNext(1);
            }
            last = last ^ 0;
            negate = 0;
            break;
        case '+':
            if(negate)
            {
                acc = acc & last;
                if(negate & 1)
                    last = !createNext(1);
                else
                    last = !!createNext(1);
            }
            last = last ^ 1;
            negate = 0;
            break;
        case '?':
            if(negate)
            {
                acc = acc & last;
                if(negate & 1)
                    last = !createNext(1);
                else
                    last = !!createNext(1);
            }
            last = last ^ -1;
            negate = 0;
            break;
        case '|':
            if(negate)
            {
                acc = acc & last;
                last = !createNext(1);
                if(!(negate & 1))
                    last = !last;
            }
            negate = 0;
            mul = mul | (acc & last);
            acc = createMatch("");
            last = createMatch("");
            break;
        default:
            acc = acc & last;
            last = createMatch(input[index]);
            if(negate)
            {
                last = !last;
                if(!(negate & 1))
                    last = !last;
            }
            negate = 0;
        }
    }
    if(negate)
    {
        acc = acc & last;
        last = !createNext(1);
        if(!(negate & 1))
            last = !last;
    }
    return mul | (acc & last);
}

Symbol::Symbol(const string& match)
    : match_(NULL), left_(NULL), right_(NULL)
{
    size_t temp = 0;
    Symbol s = Symbol::interpretString(match, temp, '\0');
    swap(s);
}

Symbol::~Symbol()
{
    delete match_;
    delete left_;
    delete right_;
}

void Symbol::swap(Symbol& b)
{
    std::swap(type_, b.type_);
    std::swap(match_, b.match_);
    std::swap(count_, b.count_);
    std::swap(left_, b.left_);
    std::swap(right_, b.right_);
}

void swap(Symbol& a, Symbol& b)
{
    a.swap(b);
}

Symbol& Symbol::operator=(Symbol rhs)
{
    swap(rhs);
    return *this;
}

Symbol Symbol::createMatch(const string& match)
{
    return Symbol(MATCH, match);
}

Symbol Symbol::createMatch(char match)
{
    string temp = "";
    temp.append(1, match);
    return Symbol(MATCH, temp);
}

Symbol Symbol::constant(const string& name, const string& value)
{
    return Symbol(name, Symbol(CONSTANT, value));
}

Symbol Symbol::createSet(const string& set)
{
    return Symbol(SET, set);
}

Symbol Symbol::createNext(int count)
{
    if(count == 0)
        return Symbol(MATCH, "");
    return Symbol(count);
}

Symbol Symbol::createLookup(const string& name)
{
    return Symbol(LOOKUP, name);
}
Match Symbol::match(const Parser& p, const string& s, size_t start, Match** table) const
{
    Match m(start, start), ret(start);
    switch(type_)
    {
    case NAMING:
        return left_->match(p, s, start, table)(*match_);
    case MATCH:
        if(start < s.length() && *match_ == s.substr(start, match_->length()))
            return Match(start, start + match_->length());
        return Match(start);
    case SET:
        if(start < s.length())
            for(size_t i = 0; i < match_->length(); i++)
                if((*match_)[i] == s[start])
                    return Match(start, start + 1);
        return Match(start);
    case REPEAT:
        if(count_ < 0)
        {
            for(int i = 0; i <= -count_; i++)
            {
                ret = left_->match(p, s, m.end(), table);
                if(ret.end() == -1)
                    return m;
                m = m << ret;
            }
            return m;
        }
        for(int i = 0; ; i++)
        {
            ret = left_->match(p, s, m.end(), table);
            if(!ret)
                return i >= count_ ? m : Match(start);
            m = m << ret;
            if((size_t)ret.end() == start)
                return m << ret;
        }
    case NOT:
        ret = left_->match(p, s, start, table);
        return ret.end() == -1 ? m : Match(start);
    case NEXT:
        return (start + count_ <= s.length()) ? Match(start, start + count_) : Match(start);
    case LOOKUP:
        return p.lookup(*match_, s, start, table);
    case CONCAT:
        ret = left_->match(p, s, start, table);
        return ret & right_->match(p, s, ret.end(), table);
    case PUSH_FIRST:
        return left_->match(p, s, start, table) >> right_->match(p, s, ret.end(), table);
    case PUSH_LAST:
        return left_->match(p, s, start, table) << right_->match(p, s, ret.end(), table);
    case EITHER:
        return left_->match(p, s, start, table) || right_->match(p, s, start, table);
    case CONSTANT:
        return Match(start, start);
    default:
        return Match(-1);
    }
}

Symbol Symbol::operator&(const Symbol& rhs) const
{
    if(type_ == MATCH && rhs.type_ == MATCH)
        return Symbol(MATCH, *match_ + *rhs.match_);
    if(type_ == MATCH && *match_ == "")
        return rhs;
    if(rhs.type_ == MATCH && *rhs.match_ == "")
        return *this;
    return Symbol(CONCAT, *this, rhs);
}

Symbol Symbol::operator|(const Symbol& rhs) const
{
    if(type_ == SET && rhs.type_ == SET)
        return Symbol(SET, *match_ + *rhs.match_);
    if(type_ == SET && *match_ == "")
        return rhs;
    if(rhs.type_ == SET && *rhs.match_ == "")
        return *this;
    return Symbol(EITHER, *this, rhs);
}

Symbol Symbol::operator>>(const Symbol& rhs) const
{
    return Symbol(PUSH_FIRST, *this, rhs);
}

Symbol Symbol::operator<<(const Symbol& rhs) const
{
    return Symbol(PUSH_LAST, *this, rhs);
}

Symbol Symbol::operator!() const
{
    if(type_ == MATCH && *match_ == "")
        return Symbol(SET, "");
    if(type_ == SET && *match_ == "")
        return Symbol(MATCH, "");
    return Symbol(NOT, *this);
}

Symbol Symbol::operator^(int amount) const
{
    return Symbol(amount, *this);
}

Symbol Symbol::operator() (const string& name) const
{
    return Symbol(name, *this);
}

ostream& Symbol::print(ostream& out, std::string tab) const
{
    out << tab;
    tab += "\t";
    switch(type_)
    {
    case NAMING:
        return left_->print(out << "name : " << *match_ << endl, tab);
    case MATCH:
        return out << "match : " << *match_ << endl;
    case SET:
        return out << "set : " << *match_ << endl;
    case REPEAT:
        return left_->print(out << "repeat : " << count_ << endl, tab);
    case NEXT:
        return out << "next : " << count_ << endl;
    case LOOKUP:
        return out << "lookup : " << *match_ << endl;
    case CONCAT:
        return right_->print(left_->print(out << "&" << endl, tab), tab);
    case PUSH_FIRST:
        return right_->print(left_->print(out << ">>" << endl, tab), tab);
    case PUSH_LAST:
        return right_->print(left_->print(out << "<<" << endl, tab), tab);
    case EITHER:
        return right_->print(left_->print(out << "|" << endl, tab), tab);
    case NOT:
        return left_->print(out << "not : " << endl, tab);
    case CONSTANT:
        return out << "constant : " << *match_ << endl;
    default:
        return out << "!!!" << endl;
    }
}

ostream& Symbol::print(ostream& out) const
{
    return print(out << endl, "");
}

ostream& operator<<(ostream& out,const Symbol& s)
{
    return s.print(out);
}
