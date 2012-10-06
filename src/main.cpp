/*
Copyright (c) 2012, Andrew Carter, Dietrich Langenbach, 
Xanda Schofield, Rai Feren

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
#include <string>
#include <iostream>
#include <unordered_set>
#include "Packrat/ast.hpp"
#include "Packrat/symbol.hpp"
#include "Packrat/parser.hpp"
#include "Packrat/pst.hpp"
#include "Packrat/build_tree.hpp"
#include "Wrapper/union.hpp"
#include "Printing/pprinter.hpp"

// Should probably use a compiler flag for this, but lazy for now.
#define VERBOSE 0

using namespace std;
using namespace packrat;
using namespace packrat::pst;
using namespace wrapper;



/*
 * Kinda dumb test function to experiment with extracting data from P
 * programs.
 *
 * Goal is to create a set of the variables in a function. Then when
 * it sees an expression, it should go "Wait, do my variables exist?",
 * and check that.
 */
int checkVars(Program& program)
{
    int varCount = 0;
    for (Function& func : program)
    {
        unordered_set<Identifier> variables; 

        string name = get<1>(func.value_);
        cerr << "Processing " << name << endl;
        Block& funcBody = get<3>(func.value_);
        for (Statement& codeLine : funcBody) 
        {
            cerr << "Line is a " << codeLine.names[codeLine.value_] << endl;

            int codeType = int(codeLine.value_);
            // Note to Rai: ...This would be better as switch. Don't be dumb. <_<
            if (codeType == 2)
            {
                Declarations declVars   = codeLine.value_.get<2>();
                list<Declaration> decls = get<1>(declVars.value_);
                for (Declaration& decl : decls)
                {
                    cerr << "Declared a " << decl.names[decl.value_] << endl;
                    if (int(decl.value_) == 0) {
                      Initializer initer = decl.value_.get<0>();
                      Expression expr = get<0>(initer.value_);
                      cerr << expr.value_.get<0>() << endl;
                      variables.insert(expr.value_.get<0>());
                    } else {
                      Expression expr = decl.value_.get<1>();
                      cerr << expr.value_.get<0>() << endl;
                      variables.insert(expr.value_.get<0>());
                    }

                    ++varCount;
                }
            }
            else if (codeType == 0)
            {
              // Simple
              Expression curExpr = 
                codeLine.value_.get<0>();
              cerr << "Furthermore, it does " << 
                curExpr.names[curExpr.value_] << endl;
            }
            else if (codeType == 1)
            { // Return
              Expression curExpr = 
                codeLine.value_.get<1>();
              cerr << "Furthermore, it does " << 
                curExpr.names[curExpr.value_] << endl;
            }

        }

    }
    return varCount;
}



int main(int argc, char* argv[])
{
    // Handle inputs
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

    // Actually build the tree

#if VERBOSE
    cerr << input << endl;
#endif
    AST temp = Parser::getPParser().parse("program",input);
#if VERBOSE
    cerr << temp << endl;
#endif

    Program func = buildTree<Program>(temp);

    // This is where we can do cool optimization stuff.
    
    int asdf = checkVars(func);
    cerr << "Declared " << asdf << " Variables." << endl;

    // Output

    if(string(argv[2]) == "-")
    {
        cout << "#include <plib.hpp>" << endl;
        cout << func << endl;
    }
    else
    {
        ofstream ofs(argv[2]);
        ofs << "#include <plib.hpp>" << endl;
        ofs << func << endl;
    }
    return 0;
}
