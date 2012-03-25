#include <string>
#include <iostream>
#include "Packrat/ast.hpp"
#include "Packrat/symbol.hpp"
#include "Packrat/parser.hpp"
// #include "Packrat/pst.hpp"
#include "Packrat/build_tree.hpp"
#include "Wrapper/union.hpp"

using namespace std;
using namespace packrat;
using namespace wrapper;

struct BinOp;

struct Value {
    typedef Union<int, BinaryExpression> type;
    static const int names_l = 1;
    typedef table_t<names_l>::type names_t;
    static names_t names;
    Union<int, BinaryExpression> value_;
};
Value::names_t Value::names = {"Int", "Sum"};

struct BinOp
{
    typedef int type;
    enum op_t { PLUS, MINUS };
    static const int names_l = 3;
    typedef table_t<names_l>::type names_t;
    static names_t names;
    type value_;
};

struct BinaryExpression
{
    typedef fold_left_t<BinaryExpression, Value, BinOp> type;
    static const int names_l = 2;
    typedef table_t<names_l>::type names_t;
    static names_t names;
    std::tuple<Value, BinOp, Value> value_;
    
    operator Value() const
    {
        Value val;
        val.value_.set<1>(*this);
        return val;
    }
};

int main(void)
{
    pst::Block b;
    Union<int, string> test;
    AST ast;
    
    ToyParser = new Parser(
        "Statement",        "<name:{String}> = <sum:{Sum}>",
        "String",           "[a-zA-Z]+",
        "Sum",              "<type=BinExp><value:{BinExp}>|<type=value><value:{Value}>",
        "BinExp",           "<value:{Value}>:(<sep:[+-]><value:{Value}>",
        "Value",            "<type=value>"    
    );
    
    packrat::pst::Expression i = buildTree<packrat::pst::Expression>(ast);
    
    test.set<1>("Hello World");
    cout << test.get<1>() << endl;
    /*
    cout << Parser::getPParser().parse("program", "int main() { a = 0 return 0; }") << endl;
    */
    return 0;
}
