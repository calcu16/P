/*
Copyright (c) 2012, 
Andrew Carter, Dietrich Langenbach, Xanda Schofield, Rai Feren

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
#define VERBOSE 1
 // 1 gives you more info on checkIfExists. 2 gets you the syntax trees.

using namespace std;
using namespace packrat;
using namespace packrat::pst;
using namespace wrapper;


bool checkIfExists(Expression curExpr, unordered_set<Identifier>& vars)
{
  bool res = true;
  string badVar = "";
  switch (int(curExpr.value_)) 
    {
    case 0:
      {
        // Identifier
        res = (vars.find(curExpr.value_.get<0>()) != vars.end()); 
#if VERBOSE
        if (res)
          cerr << "Found " ;
        else
            cerr << "ERROR: Could not find ";
        
        cerr << curExpr.value_.get<0>() << endl;
#endif
        // RAI: This block should do better stuff, but for now VERBOSE
        // covers it.
        if (not res)
          badVar = curExpr.value_.get<0>();
        break;
      }
    case 1:
      {
        // Integer.
        res = true; break;
      }
    case 2:
      {
        //Unary
        UnaryExpression curUn = curExpr.value_.get<2>();
        res = checkIfExists(get<1>(curUn.value_), vars); break;
      }
    case 3:
      {
        // Binary
        BinaryExpression curBin = curExpr.value_.get<3>();
        res = (checkIfExists(get<0>(curBin.value_), vars) && 
               checkIfExists(get<2>(curBin.value_), vars)); break;
      }
    case 4:
      {
        // call
        cerr << "Assuming function call is defined..." << endl;
        // NOTE TO RAI: Should check the environment to see if the
        // function is declared here.
        break;
      }
    case 5:
      {
        // index
        cerr << "Not yet handling indexing" << endl;
        break;
      }
    default:
      {
        cerr << "Unrecognized expression type!" << endl; 
        cerr << curExpr.value_ << endl;
        res = false; break;
      }
      
    }
  return res;
}


/*
 * Kinda dumb test function to experiment with extracting data from P
 * programs.
 *
 * Goal is to create a set of the variables in a function. Then when
 * it sees an expression, it should go "Wait, do my variables exist?",
 * and check that.
 *
 * This function is getting nasty quickly. Splitting it up some more
 * would be wonderful.
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
          //cerr << "Line is a " << codeLine.names[codeLine.value_] << endl;

            Expression curExpr;
            bool existance = true;

            switch(int(codeLine.value_))
            {
            case 0: 
              {
                // Simple
                curExpr = codeLine.value_.get<0>();
                /*
                cerr << "Furthermore, it does " << 
                        curExpr.names[curExpr.value_] << endl;
                */
                existance = checkIfExists(curExpr, variables);
                break;
              }
            case 1:
              {
                // Return 
                curExpr = codeLine.value_.get<1>();
                /*
                cerr << "Furthermore, it does " << 
                        curExpr.names[curExpr.value_] << endl;
                */
                existance = checkIfExists(curExpr, variables);
                break;
              }
            case 2:
              {
                Declarations declVars   = codeLine.value_.get<2>();
                list<Declaration> decls = get<1>(declVars.value_);
                for (Declaration& decl : decls)
                {
                  //cerr << "Declared a " << decl.names[decl.value_] << endl;
                    if (int(decl.value_) == 0) {
                      Initializer initer = decl.value_.get<0>();
                      Expression expr = get<0>(initer.value_);
#if VERBOSE
                      cerr << "Declared " << expr.value_.get<0>() << endl;
#endif
                      variables.insert(expr.value_.get<0>());
                    } else {
                      Expression expr = decl.value_.get<1>();
#if VERBOSE                      
                      cerr << "Declared " << expr.value_.get<0>() << endl;
#endif
                      variables.insert(expr.value_.get<0>());
                    }

                    ++varCount;
                }
                break;
              }
            case 3:
              {
              // If statement
              cerr << "Can not support If statements yet!" << endl;
              break;
              }
            case 4:
              {
              // for statement
              cerr << "Can not support For statements yet!" << endl;
              break;
              }
            case 5:
              {
              // Blocks
              cerr << "Derp, blocks are too much effort right now" << endl;
              break;
              }
            default:
              {
              cerr << "No idea what's going on." << endl;
              break;
              }
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

#if VERBOSE>1
    cerr << input << endl;
#endif
    AST temp = Parser::getPParser().parse("program",input);
#if VERBOSE>1
    cerr << temp << endl;
#endif

    Program func = buildTree<Program>(temp);

    // This is where we can do cool optimization stuff.
        
    int numVars = checkVars(func);
    cerr << "Declared " << numVars << " Variables." << endl;
    
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
