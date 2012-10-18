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


bool checkIfExists(Expression curExpr, unordered_set<std::string>& vars)
{
  bool res = true;
  string badVar = "";
  switch (int(curExpr.value_)) 
    {
    case 0:
      {
        // Identifier
        string ident = string(curExpr.value_.get<0>());
        res = (vars.find(ident) != vars.end()); 
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
        Call curCall = curExpr.value_.get<4>();
        for (Expression& arg : get<1>(curCall.value_)) {
          res = res && checkIfExists(arg, vars);
        }
        break;
        // NOTE TO RAI: Should check the environment to see if the
        // function is declared here.
      }
    case 5:
      {
        // index
        Index curInd = curExpr.value_.get<5>();
        res = (checkIfExists(get<0>(curInd.value_), vars) &&
               checkIfExists(get<1>(curInd.value_), vars)); break;
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


void statementCheck(Statement& codeLine,
                    unordered_set<std::string>& variables,  
                    int& varCount)
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
                Expression expr;
                if (int(decl.value_) == 0) {
                    Initializer initer = decl.value_.get<0>();
                    expr = get<0>(initer.value_);
#if VERBOSE
                    cerr << "Declared Initer for " << expr.value_.get<0>() 
                         << endl;
#endif
                    variables.insert(expr.value_.get<0>());
                    expr = get<1>(initer.value_); // for existsCheck
                } else {
                    expr = decl.value_.get<1>();
#if VERBOSE                      
                    cerr << "Declared " << expr.value_.get<0>() << endl;
#endif
                    variables.insert(expr.value_.get<0>());
                }
                existance = checkIfExists(expr, variables);
            
                ++varCount;
            }
            break;
        }
        case 3:
        {
            // If statement
            If curIf = codeLine.value_.get<3>();
            existance = checkIfExists(get<0>(curIf.value_), variables);

            unordered_set<std::string> tmpVars(variables);
            statementCheck(get<1>(curIf.value_), tmpVars, varCount);
            break;
        }
        case 4:
        {
            // for statement
            ForLoop curLoop = codeLine.value_.get<4>();
            // Need to check the init, cond, inc, and body.
            existance = checkIfExists(get<0>(curLoop.value_), variables) &&
              checkIfExists(get<1>(curLoop.value_), variables) &&
              checkIfExists(get<2>(curLoop.value_), variables);

            unordered_set<std::string> tmpVars(variables);
            statementCheck(get<3>(curLoop.value_), tmpVars, varCount);
            break;
        }
        case 5:
        {
            // Blocks
            Block curBlock = codeLine.value_.get<5>();
            for (Statement& blockLine : curBlock) {
                statementCheck(blockLine, variables, varCount);
            }
            break;
        }
        default:
        {
            cerr << "No idea what's going on." << endl;
            break;
        }
    }
}

void insertVar(Expression& pVal, unordered_set<std::string>& variables)
{
  switch(int(pVal.value_))
    {
    case 0:
      {
      Identifier ident = pVal.value_.get<0>();
      cerr << "Declared parameter: " << ident << endl;
      variables.insert(ident);
      break;
      }
    case 1:
      {
      // Have an int. This is dumb.
      cerr << "An integer was declared as a parameter. ..." << endl;
      break;
      }
    case 2:
      {
      UnaryExpression curExpr = pVal.value_.get<2>();
      insertVar(get<1>(curExpr.value_), variables);
      break;
      }
    // Not clear to me what other expressions could be doing as a
    // function parameter. MAAAAYBE BinaryExpression, but uhhh that's
    // kinda awkward.
    default:
      {
      cerr << "Unknown Expresion in function parameter." << endl;
      break;
      }
    }  
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
 *
 * NOTE: Currently does not handle paramters to functions. WHOOPS.
 */
int checkVars(Program& program)
{
    int varCount = 0;
    for (Function& func : program)
    {
        unordered_set<std::string> variables; 

        string name = get<1>(func.value_);
        cerr << "Processing " << name << endl;
        
        Parameters params = get<2>(func.value_);
        for (Parameter& param : params)
        {
          insertVar(get<2>(param.value_), variables);
        }

        Block& funcBody = get<3>(func.value_);
        for (Statement& codeLine : funcBody) 
        {
          statementCheck(codeLine, variables, varCount);
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
