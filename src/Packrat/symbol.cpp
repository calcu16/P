#include "symbol.hpp"
#include "parser.hpp"
#include "ast.hpp"
#include <string>
#include <stddef.h>
#include <iostream>
#include <sstream>

using namespace packrat;
using namespace std;
// Symbol::interpreter = Parser();

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

Symbol::Symbol(Type t, int count)
    : type_ (t), match_ (NULL), count_(count),
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

const Parser* Symbol::interpreter = NULL;

template<typename T>
static T fromString(const string& input)
{
    T a;
    istringstream(input) >> a;
    return a;
}

int Symbol::matchToInt(const AST& tree)
{
    string type = *tree["type"];
    if(type == "int")
        return matchToInt(tree["value"]);
    return fromString<int>(*tree["value"]);
}

Symbol Symbol::matchToSymbol(const AST& tree)
{
    string type = *tree["type"];
    if(type == "expr")
    {
        return matchToSymbol(tree["value"]);
    } else if(type == "ordered") {
        Symbol sum = createSet("");
        for(AST::const_iterator i = tree["value"].begin();
                    i != tree["value"].end(); ++i)
            sum = sum | matchToSymbol(*i);
        return sum;
    } else if(type == "append") {
        return matchToSymbol(tree["value"]) >> matchToSymbol(tree["tail"]);
    } else if(type == "concat") {
        Symbol prod = createMatch("");
        for(AST::const_iterator i = tree["value"].begin();
                    i != tree["value"].end(); ++i)
            prod = prod & matchToSymbol(*i);
        if(tree["eof"])
            prod = prod & !createNext(1);
        return prod;
    } else if(type == "lookup") {
        return createLookup(*tree["value"]);
    } else if(type == "next") {
        return createNext(matchToInt(tree["value"]));
    } else if(type == "any") {
        return createNext(1);
    }else if(type == "cost") {
        return createCost(matchToInt(tree["value"]));
    }else if(type == "name") {
        string op = *tree["op"], name = *tree["name"];
        if(op == "")
            return constant("")(name);
        else if(op == "=")
            return constant(*tree["value"])(name);
        else if(op == ":")
            return matchToSymbol(tree["value"])(name);
    } else if(type == "not") {
        return !matchToSymbol(tree["value"]);
    } else if(type == "rep") {
        string op = *tree["op"];
        if(op == "*")
            return matchToSymbol(tree["value"])^0;
        else if(op == "+")
            return matchToSymbol(tree["value"])^1;
        else if(op == "?")
            return matchToSymbol(tree["value"])^-1;
    } else if(type == "flatten") {
        return matchToSymbol(tree["value"]).flatten();
    } else if(type == "set") {
        Symbol sum = createSet("");
        AST options = tree["value"];
        if(tree["first"])
            options = tree["first"] >> options;
        if(tree["last"])
            options = options << tree["last"];
        for(AST::iterator i = options.begin(); i != options.end(); ++i)
            sum = sum | matchToSymbol(*i);
        if(tree["except"])
            return (!sum) & createNext(1);
        return sum;
    } else if(type == "range") {
        return createSet((*tree["left"])[0], (*tree["right"])[0]);
    } else if(type == "match") {
        return matchToSymbol(tree["value"]);
    } else if(type == "escape") {
        return createMatch((*tree["value"])[0]);
    } else if(type == "cnum") {
        return createMatch((char)matchToInt(tree["value"]));
    } else if(type == "char") {
        return createMatch(*tree["value"]);
    } else if(type == "none") {
        cerr << "Error : Unable to parse input" << endl;
        return Symbol();
    }
    cerr << "Error : Unrecognized match " << tree << endl;
    return createSet("");
}
#include <iostream>
Symbol Symbol::interpretString(const std::string& input)
{
    if(interpreter == NULL)
        interpreter = new Parser(
            "alpha", createSet('a', 'z') | createSet('A', 'Z'),
            "ddigit", createSet('0', '9'),
            "dint", constant("decimal")("type") & (!createMatch("0"))
                    & (createLookup("ddigit")^1).flatten()("value"),
            "int", constant("int")("type") & createLookup("dint")("value"),
            "anum", createMatch("_") | createLookup("alpha")
                    | createLookup("ddigit"),
            "char", constant("char")("type") & createNext(1)("value"),
            "reserved", createSet("._:|!*+?{}[]()<>\\#"),
            "range", constant("range")("type")
                    & ((createSet('a', 'z')("left") & createMatch("-")
                            & createSet('a', 'z')("right"))
                        | (createSet('A', 'Z')("left") & createMatch("-")
                            & createSet('A', 'Z')("right"))
                        | (createSet('0', '9')("left") & createMatch("-")
                            & createSet('0', '9')("right"))),
            "escape", createMatch("\\")
                    & ((constant("cnum")("type")
                        & createLookup("int")("value"))
                            | (constant("escape")("type")
                                    & createNext(1)("value"))),
            "any", constant("any")("type") & createMatch("."),
            "match", constant("match")("type")
                    & (createLookup("escape")
                        | ((!createLookup("reserved"))
                            & createLookup("char")))("value"),
            "cost", constant("cost")("type") & createMatch("#")
                    & createLookup("int")("value"),
            "set", constant("set")("type") & createMatch("[")
                    & (createMatch("^")("except")^-1)
                    & (((constant("char")("type")
                        & (createMatch("]")
                            | createMatch("-"))("value"))("first"))^-1)
                    & ((createLookup("range") 
                        | (constant("match")
                            & (!createSet("-]\\")) & createLookup("char"))
                        | createLookup("escape"))^0)("value")
                    & ((constant("char")("type")
                        & createMatch("-")("value"))("last")^-1)
                    & createMatch("]"),
            "rep", constant("rep")("type") & createLookup("atom")("value")
                    & (createMatch("*") | createMatch("+")
                        | createMatch("?"))("op"),
            "not", constant("not")("type") & createMatch("!")
                    & createLookup("maybe_not")("value"),
            "name", constant("name")("type") & createMatch("<")
                    & (createLookup("anum")^0).flatten()("name")
                    & ((createMatch("=")("op")
                            & (createLookup("anum")^0).flatten()("value"))
                        | (createMatch(":")("op")
                            & createLookup("expr")("value"))
                        | (constant("")("op")))
                    & createMatch(">"),
            "lookup", constant("lookup")("type") & createMatch("{")
                    & ((createLookup("alpha") | createMatch("_"))
                        >> (createLookup("anum")^0)).flatten()("value")
                    & createMatch("}"),
            "next",  constant("next")("type") & createMatch("{")
                    & createLookup("int")("value") & createMatch("}"),
            "subexpr", createMatch("(") & createLookup("expr")
                    & createMatch(")"),
            "atom", createLookup("match")    | createLookup("set")
                    | createLookup("subexpr")| createLookup("name")
                    | createLookup("cost")   | createLookup("next")
                    | createLookup("lookup") | createLookup("any"),
            "maybe_rep", createLookup("rep") | createLookup("atom"),
            "maybe_not", createLookup("not") | createLookup("maybe_rep"),
            "append", constant("append")("type")
                    & createLookup("maybe_not")("value")
                    & createMatch(":") & createLookup("maybe_append")("tail"),
            "maybe_append", createLookup("append") | createLookup("maybe_not"),
            "flatten", constant("flatten")("type")
                    & createLookup("maybe_append")("value")
                    & createMatch("_"),
            "maybe_flatten", createLookup("flatten")
                    | createLookup("maybe_append"),
            "concat", constant("concat")("type")
                    & (createLookup("maybe_flatten")^0)("value")
                    & (createMatch("!")("eof")^-1),
            "ordered", constant("ordered")("type") & (createLookup("concat")  >>
                    ((createMatch("|") & (createLookup("concat")))^0))("value"),
            "expr", constant("expr")("type") & createLookup("ordered")("value"),
            "input", (createLookup("expr") | constant("none")("type"))
                    & !createNext(1)
        );
    return matchToSymbol(interpreter->parse("input", input));
}

Symbol::Symbol(const string& match)
    : match_(NULL), left_(NULL), right_(NULL)
{
    Symbol s(interpretString(match));
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
Symbol Symbol::constant(const string& value)
{
    return Symbol(CONSTANT, value);
}
Symbol Symbol::constant(const string& name, const string& value)
{
    return Symbol(name, Symbol(CONSTANT, value));
}

Symbol Symbol::createSet(const string& set)
{
    return Symbol(SET, set);
}

Symbol Symbol::createSet(char s, char e)
{
    string set = "";
    for(; s <= e; s++)
        set += s;
    return Symbol(SET, set);
}

Symbol Symbol::createNext(int count)
{
    if(count == 0)
        return Symbol(MATCH, "");
    return Symbol(count);
}

Symbol Symbol::createCost(int cost)
{
    return Symbol(COST, cost);
}

Symbol Symbol::createLookup(const string& name)
{
    return Symbol(LOOKUP, name);
}
AST Symbol::match(const Parser& p, const string& s,
                    size_t start, AST** table) const
{
    AST m(start, start, ""), ret(start), other(start);
    switch(type_)
    {
    case FLATTEN:
        return left_->match(p, s, start, table).flatten();
    case NAMING:
        return left_->match(p, s, start, table)(*match_);
    case MATCH:
        if(start < s.length()
                && *match_ == s.substr(start, match_->length()))
            return AST(start, start + match_->length(), *match_);
        return AST(start);
    case SET:
        if(start < s.length())
            for(size_t i = 0; i < match_->length(); i++)
                if((*match_)[i] == s[start])
                    return AST(start, start + 1, s.substr(start, 1));
        return AST(start);
    case REPEAT:
        if(count_ < 0)
        {
            other = AST(start, start, "");
            for(int i = 0; i <= -count_; i++)
            {
                ret = left_->match(p, s, m.endc(), table);
                if(!ret)
                    return other;
                m = m << ret;
                if(other.cost() >= m.cost())
                    other = m;
            }
            return m;
        }
        for(int i = 0; i < count_; i++)
        {
            ret = left_->match(p, s, m.endc(), table);
            if(!ret)
                return other;
            if((size_t)ret.endc() == (size_t)m.endc())
                return (m << ret) << ret;
            m = m << ret;
        }
        other = m;
        for(; ;)
        {
            ret = left_->match(p, s, m.endc(), table);
            if(!ret)
                return other;
            if((size_t)ret.endc() == (size_t)m.endc())
            {
                m = (m << ret) << ret;
                if(other.cost() >= m.cost())
                    other = m;
                return other;
            }
            m = m << ret;
            if(other.cost() >= m.cost())
                other = m;
        }
    case NOT:
        ret = left_->match(p, s, start, table);
        return !ret || ret.cost() > 0 ? m : AST(start);
    case NEXT:
        return (start + count_ <= s.length())
                ? AST(start, start + max(count_,0), s.substr(start, count_))
                : AST(start);
    case LOOKUP:
        return p.lookup(*match_, s, start, table);
    case CONCAT:
        ret = left_->match(p, s, start, table);
        return ret ? ret + right_->match(p, s, ret.endc(), table) : ret;
    case PUSH_FIRST:
        ret = left_->match(p, s, start, table);
        if(ret)
            return ret >> right_->match(p, s, ret.endc(), table);
        return AST(start);
    case PUSH_LAST:
        left_->match(p, s, start, table);
        if(ret)
            return ret << right_->match(p, s, ret.endc(), table);
        return AST(start);
    case EITHER:
        ret = left_->match(p, s, start, table);
        other = right_->match(p, s, start, table);
        return !ret || (other && other.cost() < ret.cost()) ? other : ret;
    case CONSTANT:
        return AST(start, start, *match_);
    case COST:
        return AST(start, start, "", count_);
    default:
        return AST();
    }
}

Symbol Symbol::flatten() const
{
    return Symbol(FLATTEN, *this);
}

Symbol Symbol::operator&(const Symbol& rhs) const
{
    if(type_ == SET && *match_ == "")
        return *this;
    if(rhs.type_ == SET && *rhs.match_ == "")
        return rhs;
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
    if(type_ == MATCH && match_->length() == 1 
            && rhs.type_ == MATCH && rhs.match_->length() == 1)
        return Symbol(SET, *match_ + *rhs.match_);
    if(type_ == SET && rhs.type_ == MATCH && rhs.match_->length() == 1)
        return Symbol(SET, *match_ + *rhs.match_);
    if(rhs.type_ == SET && type_ == MATCH && match_->length() == 1)
        return Symbol(SET, *match_ + *rhs.match_);
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
    case COST:
        return out << "cost : " << count_ << endl;
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
        case FLATTEN:
                return left_->print(out << "flatten : " << endl, tab);
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
