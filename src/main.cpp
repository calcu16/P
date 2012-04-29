#include <string>
#include <iostream>
#include "Packrat/ast.hpp"
#include "Packrat/symbol.hpp"
#include "Packrat/parser.hpp"
#include "Packrat/pst.hpp"
#include "Packrat/build_tree.hpp"
#include "Wrapper/union.hpp"
#include "Printing/pprinter.hpp"

using namespace std;
using namespace packrat;
using namespace packrat::pst;
using namespace wrapper;

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        cout << argv[0] << " [input] [output]" << endl;
        return 1;
    }
    string input = "";
    if(string(argv[1]) == "-")
        input = string(istreambuf_iterator<char>(cin),
                       istreambuf_iterator<char>());
    else
    {
        ifstream ifs(argv[1]);
        input = string(istreambuf_iterator<char>(ifs),
                       istreambuf_iterator<char>());
    }
    
    AST temp = Parser::getPParser().parse("program",input);
    // cout << temp << endl;
    Program func = buildTree<Program>(temp);
    if(string(argv[2]) == "-")
    {
        cout << "#include ../lib/plib.hpp" << endl;
        cout << func << endl;
    }
    else
    {
        ofstream ofs(argv[2]);
        ofs << "#include ../lib/plib.hpp" << endl;
        ofs << func << endl;
    }
    return 0;
}
