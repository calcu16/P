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
//    cout << input << endl;
    AST temp = Parser::getPParser().parse("program",input);
//    cout << temp << endl;
//    return 0;
    Program func = buildTree<Program>(temp);
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
